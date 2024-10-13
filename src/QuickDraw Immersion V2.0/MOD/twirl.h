#pragma once

inline std::unordered_map<int, const char*> weapon_twirl_variation_names = {
    {0, "REVERSE_SPIN"},
    {1, "SPIN_UP"},
    {2, "REVERSE_SPIN_UP"},
    {3, "ALTERNATING_FLIPS"},
    {4, "SHOULDER_TOSS"},
    {5, "FIGURE_EIGHT_TOSS"},
};

extern int selected_single_variation;

void toggleTwirl();
void startWeaponCheck();
void checkWeaponDrawnSync();
void handleTabKey();
