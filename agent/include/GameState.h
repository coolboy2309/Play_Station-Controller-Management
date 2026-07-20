#pragma once

enum class MatchState
{
    UNKNOWN,

    WAITING,

    FIRST_HALF,

    HALFTIME,

    SECOND_HALF,

    END_CANDIDATE,

    EXTRA_TIME,

    MATCH_FINISHED
};