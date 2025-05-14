/*
 * AttachmentImplementation.cpp
 *
 *  Created on: Mar 10, 2011
 *      Author: polonel
 */

#include "server/zone/objects/tangible/attachment/Attachment.h"
#include "server/zone/ZoneServer.h"
#include "server/zone/ZoneProcessServer.h"
#include "server/zone/packets/scene/AttributeListMessage.h"
#include "server/zone/objects/creature/CreatureObject.h"
#include "server/zone/managers/loot/LootManager.h"
#include "server/zone/managers/loot/LootValues.h"

void AttachmentImplementation::initializeMembers() {
	if (gameObjectType == SceneObjectType::CLOTHINGATTACHMENT) {
		setOptionsBitmask(32, true);
		attachmentType = CLOTHINGTYPE;

	} else if (gameObjectType == SceneObjectType::ARMORATTACHMENT) {
		setOptionsBitmask(32, true);
		attachmentType = ARMORTYPE;
	}
}

void AttachmentImplementation::initializeTransientMembers() {
	TangibleObjectImplementation::initializeTransientMembers();

	setLoggingName("AttachmentObject");

	// This is here to convert the existing HashTable to a VectorMap
	if (skillModifiers.size() < 1) {
		HashTableIterator<String, int> iterator = skillModMap.iterator();

		String key = "";
		int value = 0;

		for (int i = 0; i < skillModMap.size(); ++i) {
			iterator.getNextKeyAndValue(key, value);

			skillModifiers.put(key, value);
		}
	}
}

void AttachmentImplementation::updateCraftingValues(CraftingValues* values, bool firstUpdate) {
	auto zoneServer = getZoneServer();

	if (zoneServer == nullptr) {
		return;
	}

	auto lootManager = zoneServer->getLootManager();

	if (lootManager == nullptr) {
		return;
	}

	float level = values->hasExperimentalAttribute("creatureLevel") ? values->getCurrentValue("creatureLevel") : 1;

	//NOTES The-Hunted
	//This sets the skill Mod Cap to 50
	int modCount = 1;

	for(int i = 0; i < modCount; ++i) {

		int max = (level / 12);
		int min = (level / 10) / 2;

		int mod = System::random(max) + min;

		if(mod > 50)
			mod = 50;

		if(mod < 1)
			mod = 1;

		String modName = lootManager->getRandomLootableMod(gameObjectType);

		skillModifiers.put(modName, ((mod <= 0) ? 1 : mod));
	}
}

void AttachmentImplementation::fillAttributeList(AttributeListMessage* msg, CreatureObject* object) {
	TangibleObjectImplementation::fillAttributeList(msg, object);

	StringBuffer name;

	for (int i = 0; i < skillModifiers.size(); i++) {
		auto key = skillModifiers.elementAt(i).getKey();
		auto value = skillModifiers.elementAt(i).getValue();

		name << "cat_skill_mod_bonus.@stat_n:" << key;

		msg->insertAttribute(name.toString(), value);

		if (customName.isEmpty()){

			StringId SEAName;

			SEAName.setStringId("stat_n", key);

			setCustomObjectName("", false);

			setObjectName(SEAName, false);

			setCustomObjectName(getDisplayedName() + " +" + String::valueOf(value), true);

			StringId originalName;

			if (isArmorAttachment())
				originalName.setStringId("item_n", "socket_gem_armor");

			else
				originalName.setStringId("item_n", "socket_gem_clothing");

			setObjectName(originalName, true);
		}

		name.deleteAll();
	}
}
