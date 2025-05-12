/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#include "CraftingValues.h"
#include "server/zone/objects/manufactureschematic/ManufactureSchematic.h"

CraftingValues::CraftingValues() {
	doHide = true;
	setLoggingName("CraftingValues");
	setLogging(false);
}


CraftingValues::CraftingValues(const CraftingValues& values) : Object(), Serializable(), Logger(), slots(values.slots) {
	valuesToSend = values.valuesToSend;
	doHide = values.doHide;
	schematic = values.schematic;
	player = values.player;
	attributesMap = values.attributesMap;

	setLoggingName("CraftingValues");
	setLogging(false);
}

CraftingValues::CraftingValues(const AttributesMap& values) : Object(), Serializable(), Logger() {
	doHide = true;

	int totalAttributes = values.getSize();

	for (int i = 0; i < totalAttributes; ++i) {
		String attribute = values.getAttribute(i);

		attributesMap.addExperimentalAttribute(attribute, values.getAttributeGroup(attribute), values.getMinValue(attribute), values.getMaxValue(attribute), values.getPrecision(attribute), values.isHidden(attribute), values.getCombineType(attribute));
		attributesMap.setMaxPercentage(attribute, 1.f);
	}

	setLoggingName("CraftingValues");
	setLogging(false);
}

CraftingValues::~CraftingValues() {
	schematic = nullptr;
	player = nullptr;
}

void CraftingValues::setManufactureSchematic(ManufactureSchematic* manu) {
	schematic = manu;
}

ManufactureSchematic* CraftingValues::getManufactureSchematic() {
	return schematic.get();
}

void CraftingValues::setPlayer(CreatureObject* play) {
	player = play;
}

CreatureObject* CraftingValues::getPlayer() {
	return player.get();
}

void CraftingValues::recalculateValues(bool initial, bool looted, int level) {
	// info(true) << "---------- CraftingValues::recalculateValues ----------";

	float percentage = 0.f, min = 0.f, max = 0.f, newValue = 0.f, oldValue = 0.f;
	bool hidden = false;

	// info(true) << " Total Experimental Attributes: " << getTotalExperimentalAttributes();

	for (int i = 0; i < getTotalExperimentalAttributes(); ++i) {
		String attribute = getAttribute(i);
		String group = getAttributeGroup(attribute);

		min = getMinValue(attribute);
		max = getMaxValue(attribute);

		hidden = isHidden(attribute);

		percentage = getCurrentPercentage(attribute);

		oldValue = getCurrentValue(attribute);

		// info(true) << "Attribute: " << attribute <<  " Group: " << group << " Old Value: " << oldValue << " Min: " << min << " Max: " << max;

		if (group == "") {
			if (max > min)
				newValue = max;
			else
				newValue = min;
		} else if(max != min) {
			if (max > min)
				newValue = (percentage * (max - min)) + min;
			else
				newValue = (float(1.0f - percentage) * (min - max)) + max;
		} else if(max == min) {
			newValue = max;
		}

		// info(true) << "Setting Attribute: " << attribute << " New Value: " << newValue;

				//SET THE PROTECTION FOR LOOTED ITEMS
		//The attributes below correspond the the special protection values
		//armor effectivness holds the value for all non special protection values
		if (attributeName == "armor_effectiveness" || attributeName == "blasteffectiveness" || attributeName == "heateffectiveness" ||
			attributeName == "kineticeffectiveness" || attributeName == "energyeffectiveness" || attributeName == "electricaleffectiveness" ||
			attributeName == "coldeffectiveness" || attributeName == "acideffectiveness")
		{
			if (level >= 300)
			{
				float generateRandomNumber = (float) System::random(65);
				newValue = generateRandomNumber;
			}
			else if (level >= 85 && level < 300)
			{
				float generateRandomNumber = (float) System::random(55);
				newValue = generateRandomNumber;
			}
			else if (level >= 1 && level < 85)
			{
				float generateRandomNumber = (float) System::random(35);
				newValue = generateRandomNumber;
			}
		}

		//There is no need for this if we are setting the percintages above
		//This only needs to be done to looted items
		if (attributeName == "armor_special_effectiveness" && looted == true)
		{
			newValue = 0;
		}


		if (initial || (newValue != oldValue && !initial && !hidden)) {
			setCurrentValue(attribute, newValue);
			valuesToSend.add(attribute);
		}
	}

	// info(true) << "---------- END CraftingValues::recalculateValues ----------";
}

void CraftingValues::clearAll() {
	doHide = true;
	attributesMap.removeAll();
	valuesToSend.removeAll();
	schematic = nullptr;
	player = nullptr;
	clearSlots();
}

String CraftingValues::toString() const {
	StringBuffer str;

	for (int i = 0;i < attributesMap.getSize(); ++i) {
		String attribute = attributesMap.getAttribute(i);

		str << "\n*************************" << endl;
		str << "Attribute #" << i << " Name: " << attribute << endl;
		str << "Group: " << attributesMap.getAttributeGroup(attribute) << endl;
		str << "**************************" << endl;
	}

	return str.toString();
}
