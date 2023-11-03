#include <iostream>
#include <vector>
#include "base_rule.h"

namespace mm
{

template <typename event_type>
class msg_dealer{
public:
    virtual ~msg_dealer()
    {
        typename rule_container_type::iterator i = rules_.begin();
        for(;i != rules_.end();++i){
            delete (*i);
        }
    }
    int execute(event_type * e){
        typename rule_container_type::iterator i = rules_.begin();
        for(;i != rules_.end();++i)
        {
            if(0 != (*i)->run(e)) {
                return -1;
            }
        }
    }
protected:
    event_type event_;
    typedef std::vector<i_rule<event_type> *> rule_container_type;
    rule_container_type rules_;
};

}