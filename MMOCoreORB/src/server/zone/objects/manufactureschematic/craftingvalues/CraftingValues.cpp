/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.*/

#include "CraftingValues.h"
#include "server/zone/objects/manufactureschematic/ManufactureSchematic.h"

CraftingValues::CraftingValues() {
	experimentalValuesMap.setNullValue(nullptr);
	doHide = true;
	setLoggingName("CraftingValues");
	setLogging(false);
}


CraftingValues::CraftingValues(const CraftingValues& values) : Object(), Serializable(), Logger(), slots(values.slots) {
	valuesToSend = values.valuesToSend;
	doHide = values.doHide;
	schematic = values.schematic;
	player = values.player;

	for (int i = 0; i < values.experimentalValuesMap.size(); ++i) {
		VectorMapEntry<String, Reference<Subclasses*> > entry = values.experimentalValuesMap.elementAt(i);

		Subclasses* subclass = entry.getValue();

		Subclasses* subclasses = new Subclasses(*subclass);

		experimentalValuesMap.put(entry.getKey(), subclasses);
	}

	setLoggingName("CraftingValues");
	setLogging(false);
}

CraftingValues::CraftingValues(const ValuesMap& values) : Object(), Serializable(), Logger() {
	experimentalValuesMap.setNullValue(nullptr);
	doHide = true;

	for (int i = 0; i < values.size(); ++i) {
		VectorMapEntry<String, Reference<Subclasses*> > entry = values.elementAt(i);

		Subclasses* subclass = entry.getValue();

		Subclasses* subclasses = new Subclasses(*subclass);

		experimentalValuesMap.put(entry.getKey(), subclasses);
	}

	setLoggingName("CraftingValues");
	setLogging(true);
}

CraftingValues::~CraftingValues() {
	experimentalValuesMap.removeAll();
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
	String experimentalPropTitle, attributeName;
	float percentage = 0.f, min = 0.f, max = 0.f, newValue = 0.f, oldValue = 0.f;
	bool hidden = false;

	for (int i = 0; i < getSubtitleCount(); ++i) {

		attributeName = getExperimentalPropertySubtitle(i);

		experimentalPropTitle = getExperimentalPropertyTitle(attributeName);

		min = getMinValue(attributeName);
		max = getMaxValue(attributeName);

		hidden = isHidden(attributeName);

		percentage = getCurrentPercentage(attributeName);

		oldValue = getCurrentValue(attributeName);

		if (experimentalPropTitle == "") {
			if (max > min)
				newValue = max;
			else
				newValue = min;
		} else if(max != min) {
			if (max > min)
			{
				newValue = (percentage * (max - min)) + min;
			}
			else
				newValue = (float(1.0f - percentage) * (min - max)) + max;
		} else if(max == min) {
			newValue = max;
		}

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
			info(true) << "*****newValue***** " << newValue << " Attribute Name: " << attributeName << " Initial? " << initial;
			setCurrentValue(attributeName, newValue);
			valuesToSend.add(attributeName);
		}
	}
}

void CraftingValues::clearAll() {
	doHide = true;
	experimentalValuesMap.removeAll();
	valuesToSend.removeAll();
	schematic = nullptr;
	player = nullptr;
	clearSlots();
}

String CraftingValues::toString() const {
	const Subclasses* tempSubclasses;

	StringBuffer str;

	for (int i = 0;i < experimentalValuesMap.size(); ++i) {
		tempSubclasses = experimentalValuesMap.get(i);

		str << "\n*************************" << endl;
		str << "Subclass " << i << endl;
		str << "Class: " << tempSubclasses->getClassTitle() << endl;
		str << tempSubclasses->toString();
		str << "**************************" << endl;
	}

	return str.toString();
}
