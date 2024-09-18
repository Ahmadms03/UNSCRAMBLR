//Ahmad Siddiqui
// changelog:
//   23jan23 - overall cleanup
//        changed function names
//        fixed the tail movement code
//        changed variable names
// 
// 29jan23- added more optimization
//        cleaned up functions
//        added more comments 
// 
// 30jan23- cleaned up some bugs
//          addes more functions for simplicity
//          made code look visually more appealing
// notes:
//   
//   things I need to finish debugging 

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>
#include <thread>
#include <fstream>
#include <sstream>



using namespace std;
using namespace chrono;
string useranswer;
string playerName;
char difficulty;
int numoftries = 1;
int hintsused = 0;



struct HighScore {
	string playerName;
	double score;
	char dif;

	HighScore(const string& name, double s, char d) : playerName(name), score(s), dif(d) {}
};

void addHighScore(const string& playerName, double score, char dif) {
	ofstream scoreFile("high_scores.txt", ios::app);

	if (!scoreFile) {
		cout << "Error opening file!" << endl;
		return;
	}

	scoreFile << " " << playerName << " " << score << " " << dif << endl;

	scoreFile.close();
}

void displayHighScores() {
	ifstream scoreFile("high_scores.txt");
	vector<HighScore> highScores;
	string line;

	if (!scoreFile) {
		cout << "No high scores yet!" << endl;
		return;
	}

	while (getline(scoreFile, line)) {
		stringstream ss(line);
		string playerName;
		double score;
		char dif;

		ss >> playerName >> score >> dif;
		highScores.emplace_back(playerName, score, dif);
	}

	scoreFile.close();

	// Sort the high scores in descending order
	sort(highScores.begin(), highScores.end(), [](const HighScore& a, const HighScore& b) {
		return a.score > b.score;
		});

	cout << "-------------------------------------------------------------- High Scores -------------------------------------------------------------" << endl;
	cout << "    Player Name" << "           " << "Score" << "           " << "Difficulty" << endl;
	//cout << setw(45) << left << "Difficulty" << endl;

	// Display the high scores
	
	for (const HighScore& highScore : highScores) {
		
		/*string difflevel;
		if (highScore.dif == 'E' || highScore.dif == 'e') {
			string difflevel = "Easy";
		}
		else if (highScore.dif == 'M' || highScore.dif == 'm') {
			string difflevel = "Med";
		}
		else if (highScore.dif == 'H' || highScore.dif == 'h') {
			string difflevel = "Hard";
		}
		*/
		cout << setw(15) << highScore.playerName<< setw(15) << highScore.score << setw(15) << highScore.dif << endl;
	}
}


void intro() {
	cout << "----------------------------------------------------------Welcome to UNSCRMBLR----------------------------------------------------------" << endl;
	cout << "Directions: The objectvie of the game is to unscrable the given word as fast as possible with the least number of tries" << endl;

	cout << "Press ENTER to continue" << endl;
	cin.ignore();

};

bool areStringsEqualIgnoreCase(const string& str1, const string& str2) {
	// Convert both strings to lowercase
	string lowercaseStr1 = str1;
	string lowercaseStr2 = str2;

	transform(lowercaseStr1.begin(), lowercaseStr1.end(), lowercaseStr1.begin(), ::tolower);
	transform(lowercaseStr2.begin(), lowercaseStr2.end(), lowercaseStr2.begin(), ::tolower);

	// Compare the lowercase strings
	return (lowercaseStr1 == lowercaseStr2);
}

double calculateElapsedTime(const chrono::steady_clock::time_point& startTime, const chrono::steady_clock::time_point& endTime) {
	chrono::duration<double> elapsedSeconds = endTime - startTime;
	return elapsedSeconds.count();

}


void Timer() {
	auto start = chrono::steady_clock::now();

	while (true) {
		auto now = chrono::steady_clock::now();
		auto elapsed = chrono::duration_cast<chrono::seconds>(now - start);

		cout << "\r";
		cout << "Timer: " << elapsed.count() << " seconds";
		cout.flush();

		this_thread::sleep_for(chrono::seconds(1));
	}
}

void totalPoints(char dif, double time, int attempts) {
	double totalPoints = 0;
	double points = 0;
	double TimeBonus;
	double attemptBonus;

	if (dif == 'E' || dif == 'e') {
		points = points + 100;
		attemptBonus = 50 - (attempts * 10);
		if (time > 60) {
			TimeBonus = 0;
		}
		TimeBonus = 60 - time;
	}
	else if (dif == 'M' || dif == 'm') {
		points = points + 200;
		attemptBonus = 100 - (attempts * 10);
		if (time > 100) {
			TimeBonus = 0;
		}
		TimeBonus = 100 - time;
	}
	else if (dif == 'H' || dif == 'h') {
		points = points + 300;
		attemptBonus = 100 - (attempts * 10);
		if (time > 120) {
			TimeBonus = 0;
		}
		TimeBonus = 120 - time;
	}

	int hintdeduction;
	hintdeduction = 50*(hintsused);

	totalPoints = ((points + TimeBonus + attemptBonus) - hintdeduction);

	cout << "-----------------------------------------------------------------SCORE------------------------------------------------------------------" << endl;
	cout << "points:         +" << points << endl;
	cout << "Time Bonus:     +" << TimeBonus << endl;
	cout << "Attempt Bonus:  +" << attemptBonus << endl;
	cout << "Hint deduction: -" << hintdeduction << endl;
	cout << "Total Score:    " << totalPoints << endl;
	

	addHighScore(playerName, totalPoints, dif);

}

void gameDifficulty() {
	if (difficulty == 'E' || difficulty == 'e') {
		cout << "Difficulty: Easy" << endl;
	}
	else if (difficulty == 'M' || difficulty == 'm') {
		cout << "Difficulty: Medium" << endl;
	}
	else if (difficulty == 'H' || difficulty == 'h') {
		cout << "Difficulty: Hard" << endl;
	}
}

void Gameplay(string answer) {
	auto startTime = chrono::steady_clock::now();

	while (true) {
		cin >> useranswer;
		string str1 = useranswer;
		string str2 = answer;

		/**if (useranswer == "H" || useranswer == "h") {
			int i = 0;
			for (i < answer.length(); i++;) {
				cout << answer[i] << endl;
				hintsused++;
				break;
			}
			/*if (i = answer.length()) {
				cout << "You have used all of your hints, YOU LOSE.";
				break; // Exit the loop after displaying the entire word

			}
			
			*/
			
		
	//	}
		

		//if (user)
		if ((areStringsEqualIgnoreCase(str1, str2))) {
			cout << "Correct answer!" << endl;

			auto endTime = chrono::steady_clock::now();

			cout << fixed << setprecision(2);
			double elapsedSeconds = calculateElapsedTime(startTime, endTime);
			cout << "-----------------------------------------------------------------STATS------------------------------------------------------------------" << endl;
			cout << "Time taken: " << elapsedSeconds << " seconds" << endl;
			cout << "Number of tries: " << numoftries << endl;
			cout << "Hints used: " << hintsused << endl;
			gameDifficulty();
			totalPoints(difficulty, elapsedSeconds, numoftries);
			displayHighScores();

			break;
			endTime;
			numoftries = 0;
		}
		else {
			cout << "Wrong answer. Try again." << endl;
			numoftries = numoftries + 1;
		}
	}
}


int randomWordGen(vector <string>& vec) {
	random_device rd;
	mt19937 gen(rd());

	// Generate a random index within the range of the vector size
	uniform_int_distribution<> dist(0, vec.size() - 1);
	int randomIndex = dist(gen);

	return randomIndex;
}

void wordEmitter(vector <string>& vec, int randomI) {
	string randomWord = vec[randomI];
	cout << "Here's your word: " << randomWord << endl;
	cout << "Enter the unscrambled answer: " << endl;
//	cout << "Press H for a hint, but this will deduct points!";
}

void wordAnswer(vector <string>& vec, int randomI) {
	string answer = vec[randomI];
	Gameplay(answer);
}

void runGame(vector <string>& level, vector <string>& levelAns) {
	int randomIndex = randomWordGen(level);
	wordEmitter(level, randomIndex);
	wordAnswer(levelAns, randomIndex);

};
void easyLvl() {
	vector<string> easyWords = { "toss", "blue", "luck", "beam", "open", "zoom", "tree", "jazz", "hush", "wine", "lens", "edge", "jump", "cave", "frog", "heat", "moon", "idea", "lily", "curl", "gift", "tide", "lark", "opal", "fizz", "leaf", "fire", "rose", "kiss", "yawn", "hope", "muse", "calm", "silk", "sage", "play", "star", "corn", "nose", "love", "ring", "dust", "gold", "cool", "vibe", "jade", "song", "rain", "wine", "fuzz", "lamp", "sand", "palm", "holy", "bell", "knot", "airy", "twig", "sway", "idea", "dawn", "iris", "yoga", "park", "moon", "jive", "mind", "fall", "wisp", "true", "clay", "luck", "rose", "haze", "bite", "rock", "deep", "dusk", "spin", "fern", "wave", "echo", "star", "warm", "baby", "dart", "pure", "whim", "iris", "ruby", "goat", "void" , "love"};
	vector<string> easyWordsScrmbled = { "sots", "lube", "kcul", "mabe", "pone", "mozo", "reet", "zjaz", "hush", "einw", "seln", "eged", "mpuj", "vace", "forg", "haet", "mono", "daei", "illy", "lruc", "tfig", "diet", "ralk", "lopa", "zfiz", "laef", "rife", "eros", "sisk", "wany", "eoph", "semu", "mcal", "lisj", "aegs", "alpy", "rats", "cron", "sone", "vole", "gnir", "stud", "dogl", "looc", "biev", "ejad", "gons", "nira", "einw", "zfuz", "mpla", "dsan", "lmap", "yloh", "lebl", "tonk", "iary", "wtig", "ways", "daei", "wand", "isri", "agoy", "krap", "mono", "vjie", "mdin", "llaf", "pwis", "erut", "lacy", "kcul", "eros", "zhea", "tebi", "kcor", "eepd", "kdus", "nips", "nfer", "vewa", "oech", "rats", "mwra", "ybba", "rdat", "eurp", "imhw", "isri", "ybur", "toga", "oidv", "vole"
	};
	runGame(easyWordsScrmbled, easyWords);
	



};

void medLvl() {
	vector<string> medWords = { "About", "Above", "Ample", "Angry", "Apple", "Arrow", "Bacon", "Badge", "Baker", "Basil", "Beach", "Belly", "Berry", "Bingo", "Black", "Bless", "Bliss", "Block", "Bloom", "Blues", "Bones", "Brave", "Bread", "Bride", "Brush", "Buddy", "Cabin", "Candy", "Caper", "Carat", "Cater", "Chain", "Chalk", "Champ", "Chase", "Cheek", "Chill", "Chime", "Civic", "Clean", "Click", "Cliff", "Clown", "Coach", "Coral", "Craft", "Crazy", "Crumb", "Curry", "Cycle", "Dance", "Dandy", "Darts", "Debit", "Decay", "Delta", "Dingo", "Disco", "Ditch", "Dizzy", "Donut", "Dream", "Dress", "Drift", "Drool", "Dwarf", "Eager", "Eagle", "Early", "Earth", "Elbow", "Elmly", "Email", "Ember", "Enact", "Erupt", "Every", "Extra", "Fable", "Faint", "False", "Fancy", "Favor", "Feast", "Fiber", "Fifth", "Fifty", "Final", "First", "Fixed", "Flame", "Flask", "Flock", "Floor", "Floss", "Fluid", "Flute", "Folly", "Force", "Frost", "Fruit", "Funny"

	};

	vector<string> medWordsScrmbled = { "utboa", "vaeob", "mplea", "rynag", "pplea", "rroaw", "ncoab", "gedba", "ekarb", "aslib", "eahcb", "ylelb", "yrebr", "gibno", "ckalb", "sesbl", "ssilb", "kolcb", "mobolb", "seulb", "senob", "verab", "darbe", "idebr", "hurbs", "yddub", "inabc", "danyc", "eprca", "artac", "etarc", "nihac", "halkc", "mphac", "saehc", "keehc", "hillc", "imech", "icvic", "naelc", "lickc", "fflic", "wnolc", "choac", "oralc", "raftc", "yzrac", "burmc", "yrruc", "ceylc", "nceda", "ydnad", "satrd", "bidetd", "decayd", "ateld", "odgnid", "ocdsi", "hctid", "zzyid", "odnud", "maerd", "sserd", "tirdd", "loord", "frawd", "gerae", "eagle", "yrleae", "hraet", "bwloe", "lymel", "lmiae", "breme", "tcnae", "rupet", "ryvee", "artxe", "elbaf", "tnafif", "eslaf", "acnyf", "voraf", "seaff", "biref", "fhfit", "ytfif"

	};


	runGame(medWordsScrmbled, medWords);
}


void hardLvl() {
	vector<string> hardWords = { "Freedom", "Suspend", "Fantasy", "Rapture", "Luggage", "Gesture", "Mustang", "Exhibit", "Lateral", "Bicycle", "Sampler", "Whisper", "Airport", "Harvest", "Sausage", "Playoff", "Justice", "Console", "Lantern", "Weather", "Nugget", "Compact", "Thunder", "Foresee", "Captain", "Mission", "Welcome", "Theatre", "Breathe", "Diamond", "Overdue", "Popcorn", "Curtain", "Journey", "Display", "curtain", "Typhoon", "Topaz", "Freedom", "Warrior", "Hammock", "Shelter", "Sparrow", "Snuggle", "Balance", "Foreman", "request", "Belmont", "Whisper", "Blueprint"


	};
	vector<string> hardWordsScrmbled = { "reedomf", "nsudeps", "fsyatna", "rapetur", "glugeag", "tgusree", "tunsgam", "tibixhe", "laretal", "bclieyc", "rpseaml", "ihpwser", "roptria", "vrhstae", "ssaugea", "ffoalyp", "sticuje", "solnecl", "etlarnn", "wraheet", "ngteugn", "cpotamc", "hurndet", "reefose", "picanat", "nossim", "mwelcoe", "rteaeht", "rehateb", "diamnod", "deurove", "copnrop", "ircutna", "ornjeyu", "spylaid", "turcina", "ythpoon", "apzto", "modefer", "worrari", "amkmohc", "ehserlt", "rwospar", "ngulseg", "bnaecal", "onraefm", "etrques", "lemobnt", "hpriews", "pluebritn"





	};

	runGame(hardWordsScrmbled, hardWords);


};



void levelSelect() {
	cout << "Enter your name: ";
	getline(cin, playerName);

	cout << "Please select a level" << endl;
	cout << "Press E for easy, M for Medium, and H for Hard. Then press ENTER" << endl;

	cin >> difficulty;

	if (difficulty == 'E' || difficulty == 'e') {
		easyLvl();
	}
	else if (difficulty == 'M' || difficulty == 'm') {
		medLvl();
	}
	else if (difficulty == 'H' || difficulty == 'h') {
		hardLvl();
	}
	else {
		cout << "Enter either E, M or H" << endl;
		levelSelect();
	}

}


/*
void trackAnswerTime() {
	chrono::steady_clock::time_point start, end;
	int answer;

	// Start the timer
	start = chrono::steady_clock::now();

	// Read the answer from the user
	cin >> answer;

	// Stop the timer
	end = chrono::steady_clock::now();

	// Calculate the elapsed time in seconds
	chrono::duration<double> elapsed_seconds = end - start;

	// Check if the answer is correct
	if (answer == 4) {
		cout << "Correct answer!" << endl;
		cout << "Time taken: " << elapsed_seconds.count() << " seconds" << endl;
	}
	else {
		cout << "Wrong answer!" << endl;
	}
}


*/







void totalPoints() {};

//void numbroftries();

//void timeSpent();


int main() {

	char playAgain = 'y';
	do {
		intro();
		levelSelect();
		cout << "Do you want to play again? (y/n) " << endl;
		cin >> playAgain;
		numoftries = 1;

	} while (tolower(playAgain) == 'y');

}




