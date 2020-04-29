#include "TalkingState.hpp"
#include "Messages/PhoneNumber.hpp"


namespace ue
{

TalkingState::TalkingState(Context& context, common::PhoneNumber recipient)
    :BaseState(context, "Talking State"),
      recipient(recipient)
{

}

}
