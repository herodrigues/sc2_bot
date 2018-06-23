#include <sc2api/sc2_api.h>
#include <sc2utils/sc2_manage_process.h>
#include <sc2renderer/sc2_renderer.h>

#include <iostream>

using namespace sc2;

const float kCameraWidth = 24.0f;
const int kFeatureLayerSize = 80;
const int kPixelDrawSize = 5;
const int kDrawSize = kFeatureLayerSize * kPixelDrawSize;

void DrawFeatureLayer1BPP(const SC2APIProtocol::ImageData& image_data, int off_x, int off_y) {
    assert(image_data.bits_per_pixel() == 1);
    int width = image_data.size().x();
    int height = image_data.size().y();
    renderer::Matrix1BPP(image_data.data().c_str(), width, height, off_x, off_y, kPixelDrawSize, kPixelDrawSize);
}

void DrawFeatureLayerUnits8BPP(const SC2APIProtocol::ImageData& image_data, int off_x, int off_y) {
    assert(image_data.bits_per_pixel() == 8);
    int width = image_data.size().x();
    int height = image_data.size().y();
    renderer::Matrix8BPPPlayers(image_data.data().c_str(), width, height, off_x, off_y, kPixelDrawSize, kPixelDrawSize);
}

void DrawFeatureLayerHeightMap8BPP(const SC2APIProtocol::ImageData& image_data, int off_x, int off_y) {
    assert(image_data.bits_per_pixel() == 8);
    int width = image_data.size().x();
    int height = image_data.size().y();
    renderer::Matrix8BPPHeightMap(image_data.data().c_str(), width, height, off_x, off_y, kPixelDrawSize, kPixelDrawSize);
}

class SimpleBot : public Agent {
public:
    virtual void OnGameStart() final {
        renderer::Initialize("Feature layers", 50, 50, 2 * kDrawSize, 2 * kDrawSize);
    }

    virtual void OnStep() final {
        const SC2APIProtocol::Observation* observation = Observation()->GetRawObservation();

        const SC2APIProtocol::FeatureLayers& m = observation->feature_layer_data().renders();
        DrawFeatureLayerUnits8BPP(m.unit_density(), 0, 0);
        DrawFeatureLayer1BPP(m.selected(), kDrawSize, 0);

        const SC2APIProtocol::FeatureLayersMinimap& mi = observation->feature_layer_data().minimap_renders();
        DrawFeatureLayerHeightMap8BPP(mi.height_map(), 0, kDrawSize);
        DrawFeatureLayer1BPP(mi.camera(), kDrawSize, kDrawSize);

        renderer::Render();
    }

    virtual void OnGameEnd() final {
        renderer::Shutdown();
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

    FeatureLayerSettings settings(kCameraWidth, kFeatureLayerSize, kFeatureLayerSize, kFeatureLayerSize, kFeatureLayerSize);
    coordinator.SetFeatureLayers(settings);

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
