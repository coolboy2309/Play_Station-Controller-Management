#include "../include/LayoutManager.h"

cv::Rect LayoutManager::GetTimerRegion(const std::string &competition)
{
    if (competition == "PremierLeague")
        return cv::Rect(182, 90, 47, 21);

    if (competition == "ChampionsLeague")
        return cv::Rect(103, 46, 94, 32);

    if (competition == "WorldCup")
        return cv::Rect(365, 52, 69, 29);

    if (competition == "AFCON")
        return cv::Rect(109, 79, 102, 18);

    if (competition == "CopaAmerica")
        return cv::Rect(227, 56, 80, 27);

    if (competition == "Bundesliga")
        return cv::Rect(101, 39, 64, 25);

    if (competition == "Friendly")
        return cv::Rect(109, 81, 78, 19);

    if (competition == "LaLiga")
        return cv::Rect(60, 72, 57, 36);

    if (competition == "Ligue1")
        return cv::Rect(72, 105, 65, 29);

    if (competition == "SerieA")
        return cv::Rect(115, 60, 83, 37);

    if (competition == "Euro")
        return cv::Rect(68, 35, 89, 31);

    return cv::Rect(103, 46, 94, 32);
}