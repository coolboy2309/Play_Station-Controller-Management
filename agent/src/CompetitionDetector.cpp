#include "../include/CompetitionDetector.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

std::string CompetitionDetector::Detect(const cv::Mat& image)
{
    struct CompetitionTemplate
    {
        std::string name;
        cv::Mat templ;
    };

    static std::vector<CompetitionTemplate> templates;

    if (templates.empty())
    {
        std::vector<std::string> names =
        {
            "ChampionsLeague",
            "PremierLeague",
            "WorldCup",
            "Euro",
            "SerieA",
            "Bundesliga",
            "LaLiga",
            "Ligue1",
            "AFCON",
            "CopaAmerica",
            "Friendly"
        };

        for (const auto& n : names)
        {
            CompetitionTemplate t;

            t.name = n;

            t.templ = cv::imread(
                "assets/competitions/" + n + ".jpg");

            templates.push_back(t);
        }
    }

    double bestScore = -1.0;

    std::string bestCompetition = "ChampionsLeague";

    for (const auto& item : templates)
    {
        if (item.templ.empty())
            continue;

        cv::Mat result;

        cv::matchTemplate(
            image,
            item.templ,
            result,
            cv::TM_CCOEFF_NORMED);

        double minVal;
        double maxVal;

        cv::minMaxLoc(
            result,
            &minVal,
            &maxVal);

        if (maxVal > bestScore)
        {
            bestScore = maxVal;
            bestCompetition = item.name;
        }
    }

    return bestCompetition;
}