#include <sifteo.h>
#include "EventID.h"
#include "assets.gen.h"
#include "CubeStateMachine.h"
#include "ScoredCubeState_NewWord.h"

ScoredCubeState_NewWord::ScoredCubeState_NewWord()
{
}

unsigned ScoredCubeState_NewWord::onEvent(unsigned eventID, const EventData& data)
{
    switch (eventID)
    {
    // TODO debug: case EventID_Paint:
    case EventID_EnterState:
        break;

    case EventID_NewAnagram:
        {
            VidMode_BG0 vid(getStateMachine().getCube().vbuf);
            vid.init();
            vid.BG0_drawAsset(Vec2(0,0), BGNotWordNotConnected);
            vid.BG0_text(Vec2(8,8), Font, getStateMachine().getLetters());
        }
        break;

    case EventID_AddNeighbor:
    case EventID_RemoveNeighbor:

        break;


    }
    return getStateMachine().getCurrentStateIndex();
}

unsigned ScoredCubeState_NewWord::update(float dt, float stateTime)
{
    return getStateMachine().getCurrentStateIndex();
}
