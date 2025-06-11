<!-- noashalom5@gmail.com -->
# Coup game Project

This project is an implementation of the strategy board game 'Coup', with a custom twist that introduces unique character roles and interactions. Players take turns performing actions such as gathering coins, taxing, bribing, sanctioning, arresting, and performing coups against each other. Each player is assigned a secret role at the beginning of the game that determines their special abilities and possible counters.
The game is realized by two main classes - Game and Player.  

---

## `Game` Class

The Game class is responsible for managing the logic and state of a Coup board game. It stores the list of players, tracks whose turn it is, handles the game flow, and controls rule-based behaviors such as undoing certain actions. The class ensures that the game progresses correctly, from adding players before the game starts to determining the winner at the end.

### Fields:
- `vector<Player*> playersList` – Holds pointers to all active players in the game
- `set<string> gameUsernames` – Keeps track of unique usernames to avoid duplicates.
- `int numPlayers` – The current number of players in the game.
- `int playerTurnIndex` – Index of the player whose turn it is.
- `bool gameStarted` – Indicates whether the game has officially started.

### Functions:

#### Constructor & Destructor:
- `Game()`
- `~Game()`

#### Player Managment:
- `void addPlayer(const string& username)`
- `void addPlayer(Player* player)`
- `void removePlayer(Player* player)`
Adds a player by username (with a random role) or by pointer (useful for testing).
Removes a player from the game and updates the turn index if needed.

#### Access & Utilities:
- `const vector<Player*>& getPlayersList() const`
- `Player* findPlayerByUsername(const string& username) const`
- `int getPlayerTurnIndex() const`
- `void setPlayerTurnIndex(int n)`
- `bool getGameStarted() const`
- `void setGameStarted(bool value)`
Provides access to player data, turn index, and game state.

#### Role/Action Checks:
- `bool thereIs(string type) const`
- `bool undoBribe() const`
- `bool undoCoup() const`
- `bool undoTax() const`
Checks if a player with a specific role exists.
Validates whether an action (like bribe, coup, or tax) should be undone based on role logic and game rules.

#### Turn Management:
- `Player* currentPlayer() const`
- `string turn() const`
- `void moveTurn()`
Returns the player whose turn it is, their username, and moves the game to the next turn (with special rules for bribe and merchant roles).

#### Output Functions:
- `void players() const`
- `void winner() const`
Displays the list of current players or the winner (if only one player remains)

---

## `Player` Class

The Player class represents a single player in the Coup game. Each player has a username, a role (type), a coin balance, and information about their last action and restrictions. The class manages player-specific actions such as gather, tax, bribe, arrest, sanction, and coup, and ensures the player follows the game rules

### Fields:
- `Game* game` - Pointer to the game the player is participating in.
- `string username` - The player’s unique name in the game.
- `string lastAction` - Stores the last action performed by the player.
- `string didArrest` - Stores the name of the last player arrested (to avoid repeated arrests).
- `string type` - The role of the player (Spy, Governor, Baron, Merchant, General, Judge).
- `int coins` - The current number of coins the player holds.
- `bool underSanction` - Indicates if the player is currently sanctioned.
- `bool cantDoArrest` - Restriction preventing the player from arresting during this turn.

### Functions:

#### Constructor & Destructor:
- `Player(Game* game, const string& username)`
- `virtual ~Player()`
Initializes a player with a given username and sets coin count to 0.

#### Information & Getters:
- `string getRole() const`
- `string getUsername() const`
- `int getCoins() const`
- `void setCoins(int c)`
- `string getLastAction() const`
Access player information such as username, role, last action, and coin balance.

#### Game State Checks:
- `bool myTurn() const`
- `bool tenCoins() const`
- `bool isAlive() const`
Check if it is the player's turn, if they must perform a coup, and if they are still active in the game.

#### Turn Handling:
- `void skipTurn()`
Allows the player to skip their turn (if not forced to coup). Clears the arrest restriction if needed.

#### Actions:
- `void gather()` - Gain 1 coin. Cannot be used while sanctioned (unless Baron).
- `void tax()` - Gain 2 or 3 coins based on the player's role. May be blocked by Governor. Baron under sanction gets only 1 coin.
- `void bribe()` - Costs 4 coins. May be blocked by a Judge. If successful, action is recorded.
- `void arrest(Player* p)` - Arrest another player. Special handling for Merchants and Generals. Cannot arrest the same player twice in a row.
- `void sanction(Player* p)` - Sanction another player. Costs 3 coins (4 if target is Judge). Prevents the target from gathering or taxing.
- `void coup(Player* p)` - Eliminate a player from the game. Costs 7 coins. May be blocked by a General with 5 coins.
##### Special Ability for spy and baron:
- `virtual void specialTurn()` - A virtual method to be overridden by subclasses:
Baron: Invest – Pay 3 coins, gain 6.
Spy: Either reveal another player's coins or block an arrest.

---

## Compile & Run
- `make Main` – Compiles and run the project
- `make test` – Runs the tests (from test_game.cpp and test_player.cpp)
- `make valgrind` – Checking memory leaks
- `make clean` – Remove the files from the running

- `make main` – Compile only
- `make test_exe` – Compiles test only
