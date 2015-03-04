#ifndef _BATTLE_H_INCLUDED__ //Guard the header so if it was already called once it isn't called again.
#define _BATTLE_H_INCLUDED__
/*
Made By: Patrick J. Rye
Purpose: A header to hold all the functions related to battling, levelling up and player stats.
Current Revision: 2.2
Change Log---------------------------------------------------------------------------------------------------------------------------------------------------
Date	Revision	Changed By			Changes
------  ---------   ------------		---------------------------------------------------------------------------------------------------------------------
=============================================================================================================================================================	
2/23/15	1.0			Patrick Rye			-Original
=============================================================================================================================================================
2/24/15	1.1			Patrick Rye			-Changed system("cls") to cout << string(50, '\n');
=============================================================================================================================================================
2/24/15	1.2			Patrick Rye			-Moved level up to happen when you change levels.
										-Fixed bug that would cause endless loop is you entered non integer numbers for starting stats.
											-Bug has been there since the start I just didn't know how to fix it till now.
										-Fixed bug where player could enter values below 1 for stats.
										-Fixed bug where player could enter decimal places for stats.
=============================================================================================================================================================
2/25/15	1.3			Patrick Rye			-Improved look of the code by adding separators.
										-Reduced chance for dodges and crits (cut by half).
										-Grammar and spelling fixes. (Whelp back to the old grim I guess).
										-Added breaks to case switches (kinda surprised it worked before).
=============================================================================================================================================================
2/26/15	1.4			Patrick Rye			-General code improvement.
										-Fixed losing message to better represent changes to objectives.
										-Grammar and spelling fixes.
=============================================================================================================================================================
2/27/15	1.5			Patrick Rye			-Added function that saves needed values.
										-Changed health arrays from doubles to integers.
=============================================================================================================================================================
3/2/15	1.6			Patrick Rye			-Changed name of some functions to better reflect what they do.
										-Changed health to carry over between battles.
										-Moved health calculating to its own function.
										-Moved damage calculating to its own function.
=============================================================================================================================================================
3/2/15	1.7			Patrick Rye			-Improved code.
										-Grammar and spelling fixes.
										-Moved dodge check to its own function.
=============================================================================================================================================================
3/3/15	2.0			Patrick Rye			-Grammar and spelling fixes.
										-Initialized certain variables with values.
										-Nerffered monster health, now 1/3 of a player's with the same stats.
										-Added debug mode.
										-Floored damage done when healing.
=============================================================================================================================================================
3/4/15	2.1			Patrick Rye			-Added new monster, Golem.
										-Changed monster base stats so that they total to 100.
										-Floored monster health.
=============================================================================================================================================================
3/4/15	2.2			Patrick Rye			-Change battle menu a bit.
											-Exit is now 'X' instead of 'E'.
											-Changed help to 'P' instead of 'Q'
										-Added function which can randomly return a string giving a description of the state of the monster based on current health.
										-During battle the status of the monster is replaced with description.
=============================================================================================================================================================	
*/

/*
For Stat Arrays 
0 = STR
1 = CONS
2 = DEF
3 = DEX
4 = LUK
*/

/*********************************************************************************************************/
/*A quick note on the base stats, a stat cannot be lower than 6, as a modifier might reduce the value by 5 points.
  The base stat point should also add up to be 100. */
int MonsterBaseStats[5] = {25,25,10,25,15}; //A base array for the monsters 
const int ZombieBaseStats[5] = {25,25,10,25,15};
const int SkeletonBaseStats[5] = {35,18,6,35,6};
const int WitchBaseStats[5] = {15,15,20,20,30};
const int ImpBaseStats[5] = {15,15,15,40,15};
const int GolemBaseStats[5] = {20,34,34,6,6};
/*********************************************************************************************************/
const string MonsterNames[5] = {"Zombie","Skeleton","Witch","Imp","Golem"};
const string PosMonsterModifiers[7] = {"Strong","Large","Massive","Fast","Lucky","Powerful","Solid"};
const string NegMonsterModifiers[5] = {"Weak","Small","Tiny","Slow","Unlucky"};
/*********************************************************************************************************/
string MonsterName;
string MonsterModifier;
/*********************************************************************************************************/
int PlayerHealth[2] = {0,300}; //An array 0 is current health 1 is max.
int MonsterHealth[2] = {0,300}; //An array 0 is current health 1 is max
int MonsterStats[5] = {6,6,6,6,6};
int PlayerStats[5] = {5,5,5,5,5};
/*********************************************************************************************************/
int intBattleLevel = 1;
bool blBattleDebugMode = false;
/*********************************************************************************************************/

string StateOfBeing(int intCurrHealth, int intMaxHealth)
{
	/*Outputs a string that gives a description of how the monster is doing
	  Example: at full health can return "Healthy"
	  while below 10% of max health it might return "dying" or "badly wounded"*/
	long flHealthPercent = (intCurrHealth * 100)/intMaxHealth;
	string strState;
	int intRandomState;
	
	const string FullHealthOutput[3] = {"steady","well","healthy"};
	const string SeventyPHealthOutput[3] = {"wounded","damaged","hurt"};
	const string FiftyPHealthOutput[3] = {"injured","bleeding","very hurt"};
	const string TwentyFivePHealthOutput[3] = {"really hurt","in pain","badly damaged"};
	const string TenPHealthOutput[3] = {"badly wounded","badly hurt","close to dying"};
	const string FivePHealthOutput[3] = {"to be dying","heavily wounded","gravely wounded"};
	
	intRandomState = rand() % 3; //0-2
	
	if (flHealthPercent <= 5) {strState = FivePHealthOutput[intRandomState];}
	else if (flHealthPercent <= 10) {strState = TenPHealthOutput[intRandomState];}
	else if (flHealthPercent <= 25) {strState = TwentyFivePHealthOutput[intRandomState];}
	else if (flHealthPercent <= 50) {strState = FiftyPHealthOutput[intRandomState];}
	else if (flHealthPercent <= 70) {strState = SeventyPHealthOutput[intRandomState];}
	else {strState = FullHealthOutput[intRandomState];}
	
	return strState;
}



bool DodgeCheck(int LUK, int DEX)
{
	//The way I worked out this dodge calc is that if the Dex and Luk both equal 150 (which isn't possible under the current levelling up system),
	//then they have a 25% chance to dodge. I also wanted Dex to factor into 75% of the chance and Luk only 25%
	//Can return true, that they dodged or false that they did not.
    double douDodgeChance = ((DEX/2)+(LUK/6)/4);
	if(rand() % 101 <= douDodgeChance) {return true;}
	else {return false;}
}

int CalculateHealth(int HealthLevel, int ConsStat)
{
	//A simple function for calculating health.
	//In its own function so future changes will be changed everywhere.
	return floor((23*((5.25+0.5625*HealthLevel+0.00375*pow(HealthLevel,2))+(1+0.066*HealthLevel)*(ConsStat/16))));
}

int CalculateDamage(int DamageLevel, int StrStat, int DefStat)
{
	//A simple function for calculating damage
	//In its own function so future changes will be changed everywhere.
	return floor(((((2 * (DamageLevel/5) + 2) * ((10*DamageLevel)/DefStat))*(StrStat))+5));
}

void SetBattleDebugMode(bool isDebug) {blBattleDebugMode = isDebug;}

void RandomMonster()
{
	//Generates a number 0 - 4 representing the location of a monster in the monster name array
	//It then places the name and base stats of the monster appropriately. 
	int intRandomMonsterNumber;
	
	intRandomMonsterNumber = rand() % 5;
	MonsterName = MonsterNames[intRandomMonsterNumber];
	if (MonsterName == "Witch") {for (int i=0; i <=4; i++) {MonsterBaseStats[i]=WitchBaseStats[i];}}
	else if (MonsterName == "Imp") {for (int i=0; i <=4; i++) {MonsterBaseStats[i]=ImpBaseStats[i];}}
	else if (MonsterName == "Skeleton") {for (int i=0; i <=4; i++) {MonsterBaseStats[i]=SkeletonBaseStats[i];}}
	else if (MonsterName == "Zombie") {for (int i=0; i <=4; i++) {MonsterBaseStats[i]=ZombieBaseStats[i];}}
	else if (MonsterName == "Golem") {for (int i=0; i <=4; i++) {MonsterBaseStats[i]=GolemBaseStats[i];}} 
	else
	{
		//In the event that the name wasn't found default to zombie
		MonsterName = "Zombie";
		for (int i=0; i <=4; i++) {MonsterBaseStats[i]=ZombieBaseStats[i];}
	}
}

void RandomMonsterModifier()
{
	//Adds a random modifier onto a monster it can boost or reduce the stats of the monster.
	//Varies a bit with a player's LUK
	MonsterModifier = "";
	//Two random numbers that are added onto player and monster stats
	int intMRandomNumber;
	int intPRandomNumber;
	//Two more random numbers for further randomization of the effects.
	int intRandomNumber;
	int intRandomModifier;
	intMRandomNumber = rand() % 20; //0 - 19
	intPRandomNumber = rand() % 20; //0 - 19
	intRandomNumber = rand() % 100 + 1; //1 - 100
	
	if (MonsterStats[4] + intMRandomNumber > PlayerStats[4]+intPRandomNumber)
	{
		//Monster has better chance of spawning with a positive effect
		if (intRandomNumber < 60){MonsterModifier = "";}
		else if (intRandomNumber < 90)
		{
			intRandomModifier = rand() %7; //0 - 6
			MonsterModifier = PosMonsterModifiers[intRandomModifier];
			if (MonsterModifier == "Strong") {MonsterBaseStats[0]+=5;}
			else if (MonsterModifier == "Large") {MonsterBaseStats[1]+=5;}
			else if (MonsterModifier == "Massive")
			{
				MonsterBaseStats[1]+=5;
				MonsterBaseStats[0]+=5;
				MonsterBaseStats[3]-=5;
			}
			else if (MonsterModifier == "Fast") {MonsterBaseStats[3]+=5;}
			else if (MonsterModifier == "Lucky") {MonsterBaseStats[4]+=5;}
			else if (MonsterModifier == "Powerful") {for (int i=0; i <=4; i++) {MonsterBaseStats[i]+=5;}}
			else if (MonsterModifier == "Solid") {MonsterBaseStats[1]+=5;}
			else {MonsterModifier="";}
		}
		else if (intRandomNumber <=100)
		{
			intRandomModifier = rand() %5; //0 - 4
			MonsterModifier = NegMonsterModifiers[intRandomModifier];
			if(MonsterModifier == "Weak") {MonsterBaseStats[0]-=5;}
			else if (MonsterModifier == "Small") {MonsterBaseStats[1]-=5;}
			else if (MonsterModifier == "Tiny") {MonsterBaseStats[2]-=5;}
			else if (MonsterModifier == "Slow") {MonsterBaseStats[3]-=5;}
			else if (MonsterModifier == "Unlucky") {MonsterBaseStats[4]-=5;}
			else {MonsterModifier="";}
		}
		else {/*This shouldn't happen but just in case.*/MonsterModifier="";}
	}
	else if (MonsterStats[4] + intMRandomNumber < PlayerStats[4]+intPRandomNumber)
	{
		//Monster has better chance of spawning with a negative effect
		if (intRandomNumber < 60) {MonsterModifier = "";}
		else if (intRandomNumber < 90)
		{
			intRandomModifier = rand() %5; //0 - 4
			MonsterModifier = NegMonsterModifiers[intRandomModifier];
			if(MonsterModifier == "Weak") {MonsterBaseStats[0]-=5;}
			else if (MonsterModifier == "Small") {MonsterBaseStats[1]-=5;}
			else if (MonsterModifier == "Tiny") {MonsterBaseStats[2]-=5;}
			else if (MonsterModifier == "Slow") {MonsterBaseStats[3]-=5;}
			else if (MonsterModifier == "Unlucky") {MonsterBaseStats[4]-=5;}
			else {MonsterModifier="";}
		}
		else if (intRandomNumber <=100)
		{
			intRandomModifier = rand() %7; //0 - 6
			MonsterModifier = PosMonsterModifiers[intRandomModifier];
			if (MonsterModifier == "Strong") {MonsterBaseStats[0]+=5;}
			else if (MonsterModifier == "Large") {MonsterBaseStats[1]+=5;}
			else if (MonsterModifier == "Massive")
			{
				MonsterBaseStats[1]+=5;
				MonsterBaseStats[0]+=5;
				MonsterBaseStats[3]-=5;
			}
			else if (MonsterModifier == "Fast") {MonsterBaseStats[3]+=5;}
			else if (MonsterModifier == "Lucky") {MonsterBaseStats[4]+=5;}
			else if (MonsterModifier == "Powerful") {for (int i=0; i <=4; i++) {MonsterBaseStats[i]+=5;}}
			else if (MonsterModifier == "Solid") {MonsterBaseStats[1]+=5;}
			else {MonsterModifier="";}
		}
		else {/*This shouldn't happen but just in case.*/MonsterModifier="";}
	}
	else {/*The two stats are equal therefore the monster is a normal monster without any other checks.*/MonsterModifier = "";}
//End of random monster modifier.	
}

void LevelUpFunction()
{
	//Holds function for levelling up.
	int intPlayerStatPoints = 20; //Player gets 20 skill points to spend how they would like.
	int intBattleLevelUpChoice;
	int intBattleLevelUpAmount;
	string strLevelUpChoice;
	cout << string(50, '\n');
	cout<<endl<<"LEVEL UP!"<<endl<<"You can put 20 points in any way you wish."<<endl;
	
	do
	{
		LevelUpChoice:
		cout<<"You have "<<intPlayerStatPoints<<" left to spend."<<endl<<endl;
		cout<<"STR: "<<PlayerStats[0]<<endl<<"CONS: "<<PlayerStats[1]<<endl;
		cout<<"DEF: "<<PlayerStats[2]<<endl<<"DEX: "<<PlayerStats[3]<<endl;
		cout<<"LUK: "<<PlayerStats[4]<<endl<<"NONE to not use any points.";
		cout<<endl<<"Enter the stat you wish to improve."<<endl;
		
		cout<<"> ";
		
		cin>>strLevelUpChoice;
		strLevelUpChoice = ConvertToUpper(strLevelUpChoice); //Capitalize all letters in the string.
		
		if (strLevelUpChoice == "STR") {intBattleLevelUpChoice = 0;}
		else if (strLevelUpChoice == "CONS") {intBattleLevelUpChoice = 1;}
		else if (strLevelUpChoice == "DEF") {intBattleLevelUpChoice = 2;}
		else if (strLevelUpChoice == "DEX") {intBattleLevelUpChoice = 3;}
		else if (strLevelUpChoice == "LUK") {intBattleLevelUpChoice = 4;}
		else if (strLevelUpChoice == "NONE") {intBattleLevelUpChoice = 9999;}
		else 
		{
			cout << string(50, '\n');
			cout<<endl<<"Invalid choice, try again."<<endl;
			goto LevelUpChoice;
		}
		
		if (strLevelUpChoice != "NONE")
		{
			LevelUpAmount:
			cout << string(50, '\n');
			cout<<endl<<"You have chosen to upgrade "<<strLevelUpChoice<<" please enter the points you wish to add."<<endl;
			cout<<"You have "<<intPlayerStatPoints<<" left to spend."<<endl;
			cout<<"If you chose the wrong stat just enter 0 to not give it any points."<<endl<<endl;
			cout<<"> ";
			if ( !(cin >> intBattleLevelUpAmount) ) //Checks that value entered is correct
			{
				cin.clear();
				cin.ignore();
				cout <<endl<< "Incorrect entry. Try again: ";
			}
			intBattleLevelUpAmount = floor(intBattleLevelUpAmount);
			if (intBattleLevelUpAmount > intPlayerStatPoints || intBattleLevelUpAmount < 0) 
			{
				cout << string(50, '\n');
				cout<<endl<<"You have entered an invalid number, please try again.";
				goto LevelUpAmount;
			}
			
			PlayerStats[intBattleLevelUpChoice] += intBattleLevelUpAmount;
			intPlayerStatPoints -= intBattleLevelUpAmount;
		}
		else {intPlayerStatPoints = 0;} //Player chose not to use rest of points so just cause the loop to end.
		
	} while (intPlayerStatPoints > 0);
	//Recalculate player's health.
    PlayerHealth[1] = CalculateHealth(intBattleLevel, PlayerStats[1]);
    PlayerHealth[0] = PlayerHealth[1];
//End of level up function.
}

char BattleScene() 
{
	
	/*
	Function that holds the battle scene
	Returns a T, F or E,
	T stands for True meaning that they won
	F stands for false meaning that they lost
	E stands for error meaning something went horribly wrong
	*/
	
	
    double douPlayerHealAmount;
	char chrPlayerBattleChoice;
	
	//Recalculate all of the stats needed.
    //Update monster stats to new level.
	for (int i=0; i<5; i++) {MonsterStats[i] = floor(((intBattleLevel-1)*4+MonsterBaseStats[i]));/*cout<<endl<<MonsterStats[i];*/ /*Debugging line*/}
	
    //Recalculate healths and re-heal them
    MonsterHealth[1] = floor(CalculateHealth(intBattleLevel,MonsterStats[1])/3);
    MonsterHealth[0] = MonsterHealth[1];
    //Recalculate amount player heals for.
    douPlayerHealAmount = floor(PlayerHealth[1]/10);
    BattleGoto:
	cout << string(10, '\n');

    double douPlayerCritChance = ((PlayerStats[4])/20 + rand() %3) * 4; 
    double douMonsterCritChance =((MonsterStats[4])/20 + rand() %3) * 4;
	
    double douMonsterDamageMuli = 1;
    double douPlayerDamageMuli = 1;
	
    int intPlayerDamage = 0;
    int intMonsterDamage = 0;
	
    //Check both monster and player to see if they get a crit this round.
	//Rand() % 101 generates a random number between 0 and 100.
    if (rand() % 101 <= douPlayerCritChance) {douPlayerDamageMuli = 1.375;}
    if (rand() % 101 <= douMonsterCritChance) {douMonsterDamageMuli = 1.375;}
	
	if(DodgeCheck(PlayerStats[3],PlayerStats[4])) {douMonsterDamageMuli = 0;}
    if(DodgeCheck(MonsterStats[3],MonsterStats[4])) {douPlayerDamageMuli = 0;}
	
    //Calculate damage done.
    intPlayerDamage = CalculateDamage(intBattleLevel, PlayerStats[0], MonsterStats[2]) * douPlayerDamageMuli;
    intMonsterDamage = CalculateDamage(intBattleLevel, MonsterStats[0], PlayerStats[2]) * douMonsterDamageMuli;
	
    cout<<"You are now fighting a level "<<intBattleLevel<<" "<<MonsterName<<"!";
    /*cout<<endl<<"It has "<<MonsterHealth[0]<<" out of "<<MonsterHealth[1]<<" HP left"<<endl;*/
	cout<<endl<<"The "<<MonsterName<<" appears to be "<<StateOfBeing(MonsterHealth[0],MonsterHealth[1])<<".";
    cout<<endl<<endl<<"You have "<<PlayerHealth[0]<<" out of "<<PlayerHealth[1]<<" HP left."<<endl;
    PlayerChoice:
	
    cout<<endl<<"What you like to do?"<<endl;
	cout<<"[A]ttack    [H]eal"<<endl<<"E[X]it    Hel[P]"<<endl;
	if (blBattleDebugMode) {cout<<"[K]ill monster    [D]ebug values"<<endl;}
	
    cout<<"> ";
    cin>>chrPlayerBattleChoice;
    chrPlayerBattleChoice = CharConvertToUpper(chrPlayerBattleChoice);
	
    switch(chrPlayerBattleChoice)
    {
        case 'A' :
            cout << string(10, '\n');
			
            if (intPlayerDamage != 0) //Check to see if monster manages to dodge the player.
            {
                if(douPlayerDamageMuli > 1) {cout<<"You got a crit on the "<<MonsterName<<"! ";}
                cout<<"You hit at the "<<MonsterName<<" for "<<intPlayerDamage<<".";
                MonsterHealth[0] -= intPlayerDamage;
            }
            else { cout<<"The "<<MonsterName<<" dodges your attack!"; }
            
            cout<<endl<<endl;
            if(intMonsterDamage !=0)
            {
                if(douMonsterDamageMuli > 1){cout<<"The "<<MonsterName<<" got a crit on you! ";}
                cout<<"The "<<MonsterName<<" hits you for "<<intMonsterDamage<<".";
                PlayerHealth[0] -= intMonsterDamage;
            }
            else {cout<<"You dodged the "<<MonsterName<<"'s attack!";}

            cout<<endl;
            goto HealthCheck;
			break;
        case 'H' :
            //Code for player healing.
			cout << string(10, '\n');
            if(intMonsterDamage !=0)
            {
                if(douMonsterDamageMuli > 1){cout<<"The "<<MonsterName<<" got a crit on you! ";}
                cout<<"The "<<MonsterName<<" hit you for "<<intMonsterDamage/2<<".";
                PlayerHealth[0] -= floor(intMonsterDamage/2);
            }
            else {cout<<"You dodged the "<<MonsterName<<"'s attack!";}

			if (PlayerHealth[0]+douPlayerHealAmount > PlayerHealth[1]) {PlayerHealth[0]=PlayerHealth[1];}
			else {PlayerHealth[0] += douPlayerHealAmount;}
			
            cout<<endl<<"You heal yourself for "<<douPlayerHealAmount<<" HP.";
            goto HealthCheck;
			break;
        case 'P' :
			cout << string(2, '\n');
            cout<<endl<<"Attacking means that you attack the monster and you both deal damage to each other assuming no one dodges";
            cout<<endl<<"Healing means that you heal for 10% of your maximum health, "<< douPlayerHealAmount<<" HP. While healing you also take less damage.";
			cout<<endl<<"Exit will leave the game and lose all progress.";
            cout<<endl<<"Help brings up this menu."<<endl;
            goto PlayerChoice;
			break;
        case 'D' : //Debug code reveal some values.
			if (blBattleDebugMode)
			{
				cout << string(2, '\n');
				cout<<endl<<"Monster current health: "<<MonsterHealth[0];
				cout<<endl<<"Monster max health: "<<MonsterHealth[1];
				cout<<endl<<"Player crit chance: "<<douPlayerCritChance;
				cout<<endl<<"Monster crit chance: "<<douMonsterCritChance;
				cout<<endl<<"Player muli: "<<douPlayerDamageMuli;
				cout<<endl<<"Monster muli: "<<douMonsterDamageMuli;
				cout<<endl<<"Player damage: "<<intPlayerDamage;
				cout<<endl<<"Monster damage: "<<intMonsterDamage;
			}
			else {cout<<endl<<"Invalid choice, please try again.";}
			goto PlayerChoice;
			break;
        case 'K' : //Debug code "kills" the current monster.
			if (blBattleDebugMode)
			{
				cout << string(50, '\n');
				return 'T';
			}
			else 
			{
				cout<<endl<<"Invalid choice, please try again.";
				goto PlayerChoice;
			}
			break;
        case 'X' : //Exits game.
			cout << string(50, '\n');
			cout<<endl<<"Are you sure you want to exit the game?"<<endl<<"All progress will be lost."<<endl<<"Y or N"<<endl<<"> ";
			cin>>chrPlayerBattleChoice;
			chrPlayerBattleChoice = CharConvertToUpper(chrPlayerBattleChoice);
			switch (chrPlayerBattleChoice)
			{
				case 'Y' :
					return 'F';
					break;
				default :
					goto PlayerChoice;
					break;
			}
			break;
        default :
            cout<<endl<<"Invalid choice, please try again.";
            goto PlayerChoice;
			break;
	
    }
	//Check if player is dead.
	HealthCheck:
	if (PlayerHealth[0] <= 0) {return 'F';}	
	if (MonsterHealth[0] <= 0) {return 'T';}
	//Neither player or monster is dead; go back to choice again.
	goto BattleGoto;
//End of battle scene function.
}

char PlayerInitialize()
{
	//Code for making a character, in its own function for later features.
	int intStr = 0;
	int intCons = 0;
	int intDef = 0;
	int intDex = 0;
	int intLuk = 0;

	cout<<"In this game there are five stats that effect different elements of the game.";
	cout<<endl<<"Strength (STR) - Effects how much damage you do when you attack."<<endl;
	cout<<"Constitution (CONS) - Effects how much health you have."<<endl;
	cout<<"Dexterity (DEX) - Effects if your chance to dodge."<<endl;
	cout<<"Defence (DEF) - Effects how much damage you take."<<endl;
	cout<<"Luck (LUK) - The random chance things will go your way, with dodges, crits, and rare modifiers that appear on monsters."<<endl;
	int intSkillPointsLeft = 100;
	cout<<"You have "<< intSkillPointsLeft <<" points to spend however you desire on these five stats, however each stat must have at least 1 point."<<endl;

	do 
	{
		cout<<endl<<"Enter your stat for your STRENGTH: ";
		if ( !(cin >> intStr) ) //Checks that value entered is correct
		{
			cin.clear();
			cin.ignore();
			cout << "Incorrect entry. Try again: ";
		}
		intStr = floor(intStr);
	}while (intStr < 1);
	intSkillPointsLeft -= intStr;
	//A check to see if they put too many points into a stat
	//Since each stat must have at least 1 point
	if( intSkillPointsLeft < 4 )
	{
		cout<<"You used too many points."<<endl<<"Each stat must have at least one point in it.";
		return 'F';
	}

	cout<<endl<<"You have "<< intSkillPointsLeft <<" points left to spend.";
	do 
	{
		cout<<endl<<"Enter your stat for your CONSTITUTION: ";
		if ( !(cin >> intCons) ) //Checks that value entered is correct
		{
			cin.clear();
			cin.ignore();
			cout << "Incorrect entry. Try again: ";
		}
		intCons = floor(intCons);
	}while (intCons <1);
	intSkillPointsLeft -= intCons;
	if(intSkillPointsLeft < 3)
	{
		cout<<"You used too many points."<<endl<<"Each stat must have at least one point in it.";
		return 'F';
	}
	cout<<endl<<"You have "<< intSkillPointsLeft <<" points left to spend";
	do 
	{
		cout<<endl<<"Enter your stat for your DEFENCE: ";
		if ( !(cin >> intDef) ) //Checks that value entered is correct
		{
			cin.clear();
			cin.ignore();
			cout << "Incorrect entry. Try again: ";
		}
		intDef = floor(intDef);
	}while (intDef <1);
	intSkillPointsLeft -= intDef;
	if(intSkillPointsLeft < 2)
	{
		cout<<"You used too many points."<<endl<<"Each stat must have at least one point in it.";
		return 'F';
	}
	cout<<endl<<"You have "<< intSkillPointsLeft <<" points left to spend";
	do 
	{
		cout<<endl<<"Enter your stat for your DEXTERITY: ";
		if ( !(cin >> intDex) ) //Checks that value entered is correct
		{
			cin.clear();
			cin.ignore();
			cout << "Incorrect entry. Try again: ";
		}
		intDex = floor(intDex);
	}while (intDex < 1);
	intSkillPointsLeft -= intDex;
	if(intSkillPointsLeft < 1)
	{
		cout<<"You used too many points."<<endl<<"Each stat must have at least one point in it.";
		return 'F';
	}

	cout<<endl<< intSkillPointsLeft <<" points are placed in LUCK."<<endl;
	intLuk = intSkillPointsLeft;

	char strAnswer;
	cout << string(50, '\n');
	AgreeWithStats:
	cout<<"Your current stats are as follows:"<<endl;
	cout<<"Strength: "<<intStr<<endl;
	cout<<"Constitution: "<<intCons<<endl;
	cout<<"Defence: "<<intDef<<endl;
	cout<<"Dexterity: "<<intDex<<endl;
	cout<<"Luck: "<<intLuk<<endl;
	cout<<"Do you agree with these stats? Y or N"<<endl;

    cout<<"Y or N? > ";
    cin>>strAnswer;
	strAnswer = CharConvertToUpper(strAnswer);
    switch(strAnswer)
    {
		case 'Y' :
			goto RestofGame;
			break;
		case 'N' :
			return 'F';
			break;
		default :
			cout<<endl<<"Invalid choice, try again."<<endl;
			goto AgreeWithStats;
			break;
    }

	//Make an array of the player's stats for easier referencing of them later
	RestofGame:
	PlayerStats[0]=intStr;
	PlayerStats[1]=intCons;
	PlayerStats[2]=intDef;
	PlayerStats[3]=intDex;
	PlayerStats[4]=intLuk;
	cout << string(50, '\n');
	
	PlayerHealth[1] = CalculateHealth(1,PlayerStats[1]);
    PlayerHealth[0] = PlayerHealth[1];
	
	return 'T';
//End of player initialize.
}

char startbattle(int intsLevel)
{
	intBattleLevel = intsLevel;
	char charBattleSceneEnding;
	RandomMonster(); //Generate a random monster.
	RandomMonsterModifier(); //Give monster random modifier. 
	if (MonsterModifier != "") {MonsterName = ConvertToLower(MonsterModifier) + " " + ConvertToLower(MonsterName);} //If monster has a modifier change name to include it.
	else {MonsterName = ConvertToLower(MonsterName);}
    charBattleSceneEnding = BattleScene();
	switch(charBattleSceneEnding)
	{
		case 'T' :
			cout << string(50, '\n');
			cout<<"You beat the "<<MonsterName<<"."<<endl;
            return 'T';
			break;
		case 'F' : 
			cout << string(50, '\n');
			cout<<"You lost..."<<endl<<" You completed "<<intBattleLevel - 1 <<" dungeons.";
            cout<<endl<<"Press enter to close this game and try again!";
            system("pause");
            return 'F';
			break;
		default :
			cout<<endl<<"An error has occurred in the code. Sorry :( The game will exit.";
			system("pause");
			return 'F';
			break;
	}
}

int getbattlevalue(int intvalue)
{
	if (intvalue < 0) {return 0;}
	else if (intvalue < 5) {return PlayerStats[intvalue];}
	else if (intvalue == 5) {return PlayerHealth[0];}
	else if (intvalue == 6) {return PlayerHealth[1];}
	else {return 0;}
}

void setbattlevalue(int intlocation, int intvalue)
{
	if (intlocation < 5) {PlayerStats[intlocation] = intvalue;}
	else if (intlocation == 5) {PlayerHealth[0] = intvalue;}
	else if (intlocation == 6) {PlayerHealth[1] = intvalue;}
}

#endif
