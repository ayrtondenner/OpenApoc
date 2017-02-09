#pragma once

#include "framework/stage.h"
#include "library/sp.h"
#include <map>

namespace OpenApoc
{

class Form;
class GameState;
class Palette;
class Agent;
class EquipmentPaperDoll;

class AEquipScreen : public Stage
{
  private:
	sp<Form> form;
	sp<Palette> pal;
	sp<GameState> state;
	sp<Agent> currentAgent;

	sp<EquipmentPaperDoll> paperDoll;

	static const Vec2<int> EQUIP_GRID_SLOT_SIZE;
	static const Vec2<int> EQUIP_GRID_SLOTS;

  public:
	AEquipScreen(sp<GameState> state);
	~AEquipScreen() override;

	void begin() override;
	void pause() override;
	void resume() override;
	void finish() override;
	void eventOccurred(Event *e) override;
	void update() override;
	void render() override;
	bool isTransition() override;

	void setSelectedAgent(sp<Agent> agent);
};

} // namespace OpenApoc
