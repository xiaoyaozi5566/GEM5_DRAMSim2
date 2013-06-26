#ifndef __PARAMS__SimObject__
#define __PARAMS__SimObject__

class SimObject;

#ifndef PY_VERSION
struct PyObject;
#endif

#include <string>

class EventQueue;

struct SimObjectParams
{
        SimObjectParams()
        {
            extern EventQueue mainEventQueue;
            eventq = &mainEventQueue;
        }
        virtual ~SimObjectParams() {}

        std::string name;
        PyObject *pyobj;
        EventQueue *eventq;
                
};

#endif // __PARAMS__SimObject__
