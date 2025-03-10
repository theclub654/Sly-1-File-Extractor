#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

std::string levelNames[45] = { "Paris.brx", "The Hideout.brx", "A Stealthy Approach.brx", "Prowling the Grounds.brx" ,"High Class Heist.brx", "Into the Machine.brx", "A Cunning Disguise.brx"
		, "The Fire Down Below.brx", "Treasure in the Depths.brx", "The Gunboat Graveyard.brx", "The Eye of the Storm.brx", "A Rocky Start.brx", "Muggshot's Turf.brx", "Boneyard Casino.brx", "Murray's Big Gamble.brx"
		, "At the Dog Track.brx", "Two to Tango.brx", "Straight to the Top.brx", "Back Alley Heist.brx", "Last Call.brx", "The Dread Swamp Path.brx", "The Swamp's Dark Center.brx", "The Lair of the Beast.brx"
		, "A Grave Undertaking.brx", "Piranha Lake.brx", "Descent into Danger.brx", "A Ghastly Voyage.brx", "Down Home Cooking.brx", "A Deadly Dance.brx", "A Perilous Ascent.brx", "Inside the Stronghold.brx"
		, "Flaming Temple of Flame.brx", "The Unseen Foe.brx", "The King of the Hill.brx", "Rapid Fire Assault.brx", "Duel by the Dragon.brx", "A Desperate Race.brx", "Flame Fu!.brx", "A Hazardous Path.brx"
		, "Burning Rubber.brx", "A Daring Rescue.brx", "Bentley Comes Through.brx", "A Temporary Truce.brx", "Sinking Peril.brx", "A Strange Reunion.brx" };

constexpr size_t CHUNK_SIZE = 0x2000;
constexpr size_t CHUNK_SIZE_MASK = CHUNK_SIZE - 1;

int  main(int argc, char** argv);
void Decompress(std::vector <char> &filebuffer, uint64_t size ,std::vector <unsigned char> &outputData);