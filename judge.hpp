// noashalom5@gmail.com
#include <string>
using namespace std;

namespace coup {
    
class Judge : public Player { 
    public:
        Jugde(const string& game, const string& name)
            : Player(game, name) {
                type = "Judge";
            }
        virtual ~Judge() override {}
        
        bool wantsToUndoBribe() const;
    };
}