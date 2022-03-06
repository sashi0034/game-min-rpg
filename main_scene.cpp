
#include "main_scene.h"
#include "start.h"

namespace ingame::main
{
    /// <summary>
    /// �t�B�[���h��̏���̊��
    /// </summary>
    /// <param name="x">�s��X</param>
    /// <param name="y">�s��Y</param>
    FieldDecorationBase::FieldDecorationBase(int x, int y) : Actor()
    {
        mSpr->SetZ(double(ZIndex::FLOOR) - 1);
        mSpr->SetXY(x * 16, y * 16);
    }

    Weed::Weed(int x, int y) : FieldDecorationBase(x, y) {}
    void Weed::update()
    {
        mSpr->SetImage(Images->Weed, ((int)(mTime / 1000) % 2) * 16, 0, 16, 16);
        Actor::update();
    }

    Tree::Tree(int x, int y) : FieldDecorationBase(x, y) {}
    void Tree::update()
    {
        mSpr->SetImage(Images->Tree, ((int)(mTime / 1000 * 2) % 4) * 16, 0, 16, 16);
        Actor::update();
    }
}





