#include "filter/filter.h"

#include <print>
#include <filesystem>
#include <memory>
#include <vector>
#include <string>
#include <fstream>
#include <ranges>
#include <stdexcept>

struct FilterDeleter
{
    void operator()(filter_t* ctx) const
    {
        filter_free(ctx);
    }
};

using Filter = std::unique_ptr<filter_t, FilterDeleter>;

struct Point
{
    Point(const std::string& line)
    {
        unmarshal(line);
    }

    void unmarshal(const std::string& line)
    {
        auto items = line | std::views::split(',') | std::ranges::to<std::vector<std::string>>();
        if (items.size() < 2)
        {
            throw std::runtime_error("Unexpected Point line format");
        }

        time = std::stof(items[0]);
        input = std::stof(items[1]);

        // std::print("Point: time[{:08f}] input[{:08f}]\n", time, input);
    }

    void marshal(std::ofstream& file) const
    {
        file << std::format("{:.3f},{:.6f},{:.6f}\n", time, input, output);
    }

    float time = 0.0f;
    float input = 0.0f;
    float output = 0.0f;
};

class CsvFile
{
public:
    CsvFile(const std::string& filePath)
    {
        auto file = std::ifstream(filePath, std::ios::binary | std::ios::in);
        if (!file.is_open())
        {
            throw std::runtime_error(std::format("Error file cannot be opened or accessed: {:s}\n", filePath));
        }

        std::print("[CsvFile] Opened: {:s}\n", filePath);

        file.seekg(0, std::ios::end);
        const std::size_t fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        /*
            Expected format on input:
            t,value
            0.000000,0.091415
        */

        bool headerSeen = false;
        std::string line;
        while (!file.eof())
        {
            std::getline(file, line);
            if (line.empty())
            {
                break;
            }

            if (!headerSeen)
            {
                headerSeen = true;
                continue;
            }

            m_points.emplace_back(line);
        }

        std::print("[CsvFile] Total points loaded: {:d}\n", m_points.size());
    }

    auto getTimeSequence()
    {
        std::vector<float> out;
        for (const auto& point : m_points)
        {
            out.push_back(point.time);
        }
        return out;
    }

    auto getInputSequence()
    {
        std::vector<float> out;
        for (const auto& point : m_points)
        {
            out.push_back(point.input);
        }
        return out;
    }

    void setOutputSequence(const std::vector<float>& outputBuffer)
    {
        if (outputBuffer.size() != m_points.size())
        {
            std::print("Error output buffer size does not match\n");
            return;
        }

        auto outputIter = outputBuffer.begin();

        for (auto& point : m_points)
        {
            point.output = *outputIter;
            std::advance(outputIter, 1);
        }
    }

    void save(const std::string& filePath) const
    {
        auto file = std::ofstream(filePath, std::ios::binary | std::ios::out);
        if (!file.is_open())
        {
            throw std::runtime_error(std::format("Error creating file: {:s}\n", filePath));
        }

        file << "t,raw,filtered\n";
        for (const auto& point : m_points)
        {
            point.marshal(file);
        }
    }

private:
    std::vector<Point> m_points;
};

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::print("Usage: {:s} <csv file path>\n", argv[0]);
        return -1;
    }

    auto path = std::filesystem::path(argv[1]);
    if (!std::filesystem::exists(path))
    {
        std::print("Error selected file does not exist: {:s}\n", path.string());
        return -1;
    }

    auto csvFile = CsvFile(path.string());

    auto filter = Filter(filter_new(40));
    // do filtering

    auto outputSamples = std::vector<float>(csvFile.getTimeSequence().size());
    auto outIter = outputSamples.begin();

    for (const auto& sample : csvFile.getInputSequence())
    {
        *outIter = filter_process_single(filter.get(), sample);
        std::advance(outIter, 1);
    }

    csvFile.setOutputSequence(outputSamples);

    csvFile.save(path.replace_filename("output.csv"));

    return 0;
}
