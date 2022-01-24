#pragma once

namespace app
{
    namespace xgame
    {
        class MsgPLChangeAreaTopView : public fnd::Message
        {
        public:
            bool IsTopView{};

            MsgPLChangeAreaTopView() : Message()
            {
                Type = fnd::PROC_MSG_PL_CHANGE_AREA_TOP_VIEW;
            }

            MsgPLChangeAreaTopView(bool isTopView) : MsgPLChangeAreaTopView()
            {
                IsTopView = isTopView;
            }
        };
    }

}