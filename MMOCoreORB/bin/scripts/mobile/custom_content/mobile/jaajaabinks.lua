jaajaabinks = Creature:new {
	objectName = "",
	customName = "Jar Jar Binks",
	socialGroup = "Dark Jedi",
	faction = "",
	mobType = MOB_NPC,
	level = 280,
	chanceHit = 72.5,
	damageMin = 200,
	damageMax = 310,
	baseXp = 27849,
	baseHAM = 60000,
	baseHAMmax = 89000,
	armor = 0,
	resists = {0, 0, 0, 0, 0, 0, 0, 0, 0},
	meatType = "",
	meatAmount = 0,
	hideType = "",
	hideAmount = 0,
	boneType = "",
	boneAmount = 0,
	milk = 0,
	tamingChance = 0,
	ferocity = 0,
	pvpBitmask = AGGRESSIVE + ATTACKABLE + ENEMY,
	creatureBitmask = KILLER + STALKER,
	optionsBitmask = 128,
	diet = HERBIVORE,
	scale = 1,
	customAiMap = "",

	templates = {
		"object/mobile/gungan_s03_male.iff"
	},

	lootGroups = {
		{
			groups = {
				{group = "krayt_pearls", chance = 10000000}
			},
		}
	},

	-- Primary and secondary weapon should be different types (rifle/carbine, carbine/pistol, rifle/unarmed, etc)
	-- Unarmed should be put on secondary unless the mobile doesn't use weapons, in which case "unarmed" should be put primary and "none" as secondary
	primaryWeapon = "mixed_force_weapons",
	secondaryWeapon = "none",

	-- primaryAttacks and secondaryAttacks should be separate skill groups specific to the weapon type listed in primaryWeapon and secondaryWeapon
	-- Use merge() to merge groups in creatureskills.lua together. If a weapon is set to "none", set the attacks variable to empty brackets
	primaryAttacks = merge(pikemanmaster,brawlermaster,fencermaster),
	secondaryAttacks = {},
	conversationTemplate = ""
}

CreatureTemplates:addCreatureTemplate(jaajaabinks, "jaajaabinks")