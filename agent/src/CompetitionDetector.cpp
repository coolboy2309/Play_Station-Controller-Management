
#include "../include/CompetitionDetector.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>

std::string CompetitionDetector::Detect(const cv::Mat& image)
{
    using Clock = std::chrono::steady_clock;
    auto start = Clock::now();

    if (image.empty())
        return "Unknown";

    struct CompetitionTemplate
    {
        std::string name;
        cv::Mat color;
        cv::Mat gray;
    };

    struct Candidate
    {
        int index;
        double score;
        cv::Point location;
    };

    static std::vector<CompetitionTemplate> templates;
    static bool initialized = false;

    if (!initialized)
    {
        const std::vector<std::string> names =
        {
            "ChampionsLeague", "PremierLeague", "WorldCup",
            "Euro", "SerieA", "Bundesliga", "LaLiga",
            "Ligue1", "AFCON", "CopaAmerica", "Friendly"
        };

        int loaded = 0;

        for (const auto& name : names)
        {
            CompetitionTemplate item;
            item.name = name;

            item.color = cv::imread(
                "assets/competitions/" + name + ".jpg",
                cv::IMREAD_COLOR
            );

            if (!item.color.empty())
            {
                cv::cvtColor(
                    item.color,
                    item.gray,
                    cv::COLOR_BGR2GRAY
                );

                loaded++;
            }

            templates.push_back(item);
        }

        initialized = true;

        std::cout << "[INFO] Competition Templates Loaded: "
                  << loaded << "/" << names.size() << std::endl;
    }

    // Stage 1: Search the broad scoreboard region in grayscale.
    int width = std::min(image.cols, 500);
    int height = std::min(image.rows, 200);

    if (width <= 0 || height <= 0)
        return "Unknown";

    cv::Mat searchColor =
        image(cv::Rect(0, 0, width, height));

    cv::Mat searchGray;

    cv::cvtColor(
        searchColor,
        searchGray,
        cv::COLOR_BGR2GRAY
    );

    std::vector<Candidate> candidates;

    for (int i = 0; i < static_cast<int>(templates.size()); i++)
    {
        const auto& item = templates[i];

        if (item.color.empty() || item.gray.empty())
            continue;

        if (item.gray.cols > searchGray.cols ||
            item.gray.rows > searchGray.rows)
            continue;

        cv::Mat result;

        cv::matchTemplate(
            searchGray,
            item.gray,
            result,
            cv::TM_CCOEFF_NORMED
        );

        double minVal, maxVal;
        cv::Point minLoc, maxLoc;

        cv::minMaxLoc(
            result,
            &minVal,
            &maxVal,
            &minLoc,
            &maxLoc
        );

        candidates.push_back({ i, maxVal, maxLoc });
    }

    if (candidates.empty())
        return "Unknown";

    // Keep only the three strongest grayscale candidates.
    std::sort(
        candidates.begin(),
        candidates.end(),
        [](const Candidate& a, const Candidate& b)
        {
            return a.score > b.score;
        }
    );

    if (candidates.size() > 3)
        candidates.resize(3);

    // Stage 2: Verify the candidates using original color images.
    double bestScore = -1.0;
    std::string bestCompetition = "Unknown";

    constexpr int SEARCH_MARGIN = 8;

    for (const auto& candidate : candidates)
    {
        const auto& item = templates[candidate.index];

        int x = std::max(
            0, candidate.location.x - SEARCH_MARGIN
        );

        int y = std::max(
            0, candidate.location.y - SEARCH_MARGIN
        );

        int right = std::min(
            width,
            candidate.location.x +
            item.color.cols + SEARCH_MARGIN
        );

        int bottom = std::min(
            height,
            candidate.location.y +
            item.color.rows + SEARCH_MARGIN
        );

        cv::Rect verifyRect(
            x, y, right - x, bottom - y
        );

        cv::Mat verifyRegion = searchColor(verifyRect);

        if (verifyRegion.cols < item.color.cols ||
            verifyRegion.rows < item.color.rows)
            continue;

        cv::Mat result;

        cv::matchTemplate(
            verifyRegion,
            item.color,
            result,
            cv::TM_CCOEFF_NORMED
        );

        double minVal, maxVal;

        cv::minMaxLoc(
            result,
            &minVal,
            &maxVal
        );

        if (maxVal > bestScore)
        {
            bestScore = maxVal;
            bestCompetition = item.name;
        }
    }

    constexpr double CONFIDENCE_THRESHOLD = 0.70;

    if (bestScore < CONFIDENCE_THRESHOLD)
        bestCompetition = "Unknown";

    double elapsed =
        std::chrono::duration<double, std::milli>(
            Clock::now() - start
        ).count();

    std::cout << "[INFO] Color Verification Score: "
              << std::fixed << std::setprecision(3)
              << bestScore << std::endl;

    std::cout << "[INFO] Competition Result: "
              << bestCompetition << std::endl;

    std::cout << "[PERF] Competition Detection: "
              << elapsed << " ms" << std::endl;

    return bestCompetition;
}
