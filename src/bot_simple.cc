#include <sc2api/sc2_api.h>

#include <iostream>

using namespace sc2;

class Bot : public Agent {
public:
    virtual void OnGameStart() final {
        std::cout << "Hello, World!" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    Coordinator coordinator;
    coordinator.LoadSettings(argc, argv);

    Bot bot;
    coordinator.SetParticipants({
        CreateParticipant(Race::Terran, &bot),
        CreateComputer(Race::Zerg)
    });

    coordinator.LaunchStarcraft();
	const char* kMapAbyssalReefLE = "Ladder2017Season3/AbyssalReefLE.SC2Map";
    coordinator.StartGame(kMapAbyssalReefLE);

    while (coordinator.Update()) {
    }

    return 0;
}