#include <sc2api/sc2_api.h>
#include <sc2utils/sc2_manage_process.h>
#include <sc2renderer/sc2_renderer.h>
#include <iostream>

using namespace sc2;

const float kCameraWidth = 24.0f;
const int kFeatureLayerSize = 80;
const int kPixelDrawSize = 5;
const int kDrawSize = kFeatureLayerSize * kPixelDrawSize;

class SimpleBot : public Agent {
public:
	virtual void OnStep() final {
		std::cout << Observation()->GetGameLoop() << std::endl;
	}

    virtual void OnUnitIdle(const Unit* unit) final {
		switch (unit->unit_type.ToType()) {
			case UNIT_TYPEID::TERRAN_COMMANDCENTER: {
				Actions()->UnitCommand(unit, ABILITY_ID::TRAIN_SCV);
				break;
			}
			case UNIT_TYPEID::TERRAN_SCV: {
				const Unit* mineral_target = FindNearestMineralPatch(unit->pos);
				if (!mineral_target) {
					break;
				}
				Actions()->UnitCommand(unit, ABILITY_ID::SMART, mineral_target);
				break;
			}
			case UNIT_TYPEID::TERRAN_MARINE: {
				const Unit* enemy_target = FindNearestEnemy(unit->pos);
				if (!enemy_target) {
					break;
				}
				Actions()->UnitCommand(unit, ABILITY_ID::ATTACK_ATTACK, enemy_target);
			}
			default: {
				break;
			}
		}
	}

	const Unit* FindNearestMineralPatch(const Point2D& start) {
		Units units = Observation()->GetUnits(Unit::Alliance::Neutral);
		float distance = std::numeric_limits<float>::max();
		const Unit* target = nullptr;
		for (const auto& u : units) {
			if (u->unit_type == UNIT_TYPEID::NEUTRAL_MINERALFIELD) {
				float d = DistanceSquared2D(u->pos, start);
				if (d < distance) {
					distance = d;
					target = u;
				}
			}
		}
		return target;
	}

	const Unit* FindNearestEnemy(const Point2D& start) {
		Units units = Observation()->GetUnits(Unit::Enemy);
		float distance = std::numeric_limits<float>::max();
		const Unit* target = nullptr;
		for (const auto& u : units) {
			if (u->unit_type == UNIT_TYPEID::ZERG_ROACH) {
				float d = DistanceSquared2D(u->pos, start);
				if (d < distance) {
					distance = d;
					target = u;
				}
			}
		}
		return target;
	}
};

int main(int argc, char* argv[]) {
    Coordinator coordinator;
    if (!coordinator.LoadSettings(argc, argv)) {
        return 1;
    }

    // Add the custom bot, it will control the players.
    SimpleBot bot;

    coordinator.SetParticipants({
        CreateParticipant(Race::Terran, &bot),
        CreateComputer(Race::Zerg)
    });

    // Start the game.
    coordinator.LaunchStarcraft();
	const char* kMapCollectMineralsAndGas = "mini_games/CollectMineralsAndGas.SC2Map";
	coordinator.StartGame(kMapCollectMineralsAndGas);

    while (coordinator.Update()) {
        if (PollKeyPress()) {
            break;
        }
    }

    return 0;
}
