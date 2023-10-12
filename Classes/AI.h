#pragma once

#include "Controller.h"

class AI : public Controller {
};

class CampAI : public AI {
};

class PlayerAI : public AI {
};

class RandomPlayerAI : public PlayerAI {
};

class NpcAI : public AI {
};
