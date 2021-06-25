#pragma once

namespace app
{
    class CGameSequence
    {
    private: 
        inline static FUNCTION_PTR(int, __thiscall, f_PushMovieEventQueue, ASLR(0x0090DBE0), CGameSequence* This, int eventNo);
        inline static FUNCTION_PTR(int, __thiscall, f_SeqGotoPlayMovie, ASLR(0x00911B70), CGameSequence* This);

    public:
        static void CompleteDLCStage();

        int PushMovieEventQueue(int eventNo) { return f_PushMovieEventQueue(this, eventNo); }
        int SeqGotoPlayMovie() { return f_SeqGotoPlayMovie(this); }
    };
}