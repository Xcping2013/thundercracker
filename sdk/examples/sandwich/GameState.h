#pragma once
#include "Content.h"

class GameState {
private:
	uint32_t mQuest;
	uint32_t mQuestMask;
	uint32_t mUnlockMask;
  	uint32_t mItemSet;

public:
	void Init();
	bool AdvanceQuest();
	const QuestData* Quest() const { return gQuestData+mQuest; }
	bool FlagTrigger(const TriggerData& trigger);
	bool AllQuestsComplete() { return mQuest == gQuestCount; }
	bool IsActive(const TriggerData& trigger) const;
	bool HasAnyItems() const { return mItemSet != 0; }
  	bool PickupItem(int itemId);
  	bool HasItem(int itemId) const { return (mItemSet & (1<<itemId)) != 0; }
  	unsigned GetItems(uint8_t* buf); // buf must have a capacity of 16 elements


private:
	bool Flag(uint8_t questId, uint8_t flagId);
	bool IsActive(uint8_t questId, uint8_t flagId) const;


	void Save();
	void Load();
};