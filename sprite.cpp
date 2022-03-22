#include "stdafx.h"







std::vector<Sprite*> Sprite::sprites = std::vector<Sprite*>();


void Sprite::Init()
{
    sprites = std::vector<Sprite*>();
}


void Sprite::End()
{
    Sprite::DestroyAll();
}


Sprite::Sprite() : Sprite(nullptr, 0, 0, 0, 0)
{}
Sprite::Sprite(Graph* image) : Sprite(image, 0, 0, 0, 0)
{
    int x = 0, y = 0;
    DxLib::GetGraphSize(image->GetHandler(), &x, &y);
    this->width = x;
    this->height = y;
}
Sprite::Sprite(Graph* image, int u, int v, int w, int h)
{
    this->image = image;
    this->u = u;
    this->v = v;
    this->width = w;
    this->height = h;
    this->drawingMethod = Sprite::DrawingKind::AlignGrid;

    sprites.push_back(this);
}
Sprite::~Sprite()
{
    //std::cout OUT_LOG "Sprite deleted " << this;
}



void Sprite::SetFlip(bool isFlip)
{
    this->isFlip = isFlip;
}
bool Sprite::GetFlip()
{
    return this->isFlip;
}

void Sprite::SetImage(Graph* image)
{
    this->image = image;
}
void Sprite::SetImage(int u, int v)
{
    this->u = u;
    this->v = v;
}
void Sprite::SetImage(int u, int v, int width, int height)
{
    this->SetImage(u, v);
    this->width = width;
    this->height = height;
}
void Sprite::SetImage(Graph* image, int u, int v, int width, int height)
{ 
    this->SetImage(image);
    this->SetImage(u, v, width, height);
}
void Sprite::GetImage(Graph** image, int* u, int* v, int* width, int* height)
{
    *image = this->image;
    *u = this->u;
    *v = this->v;
    *width = this->width;
    *height = this->height;
}

void Sprite::SetScale(double scale)
{
    this->scale = scale;
}

double Sprite::GetScale()
{
    return this->scale;
}


void Sprite::SetXY(double x, double y)
{
    this->x = x;
    this->y = y;
}
void Sprite::GetXY(double* x, double* y)
{
    *x = this->x;
    *y = this->y;
}

void Sprite::SetZ(double z)
{
    this->z = z;
}
double Sprite::GetZ()
{
    return this->z;
}


void Sprite::GetScreenXY(int* x, int* y)
{
    double x1, y1;
    this->GetLinkDifferenceXY(&x1, &y1);

    *x = int(this->x + x1);
    *y = int(this->y + y1);
}


void Sprite::SetRotationDeg(double deg)
{
    this->rotationRad = (deg / 180.0) * M_PI;
}
void Sprite::SetRotationRad(double rad)
{
    this->rotationRad = rad;
}
double Sprite::GetRotationRad()
{
    return this->rotationRad;
}


void Sprite::SetBelong(std::any instance)
{
    this->belong = instance;
}

std::any Sprite::GetBelong()
{
    return this->belong;
}

void Sprite::SetLinkXY(const Sprite* linkSpr)
{
    this->linkXY = linkSpr;
}
Sprite* Sprite::GetLinkXY()
{
    return const_cast<Sprite*>(this->linkXY);
}
/// <summary>
/// Disposeするタイミングの同期
/// </summary>
/// <param name="linkSpr"></param>
void Sprite::SetLinkAlive(const Sprite* linkSpr)
{
    if (this->linkAlive != nullptr)
    {
        Sprite* oldParent = const_cast<Sprite*>(this->linkAlive);
        oldParent->linkedChildAlives.erase(this);
    }
    Sprite* parent = const_cast<Sprite*>(linkSpr);
    this->linkAlive = linkSpr;
    parent->linkedChildAlives.insert(this);
}


void Sprite::GetLinkDifferenceXY(double* x, double* y)
{
    const Sprite* linkSpr = this->linkXY;
    if (linkSpr != nullptr)
    {
        double x1, y1;
        const_cast<Sprite*>(linkSpr)->GetLinkDifferenceXY(&x1, &y1);
        *x = linkSpr->x + x1;
        *y = linkSpr->y + y1;
        return;
    }
    else
    {
        *x = 0;
        *y = 0;
        return;
    }
}


void Sprite::SetBlend(int blendMode, int blendPal)
{
    this->SetBlendMode(blendMode);
    this->SetBlendPal(blendPal);
}
void Sprite::SetBlendMode(int blendMode)
{
    this->blendMode = blendMode;
}
void Sprite::SetBlendPal(int blendPal)
{
    this->blendPal = blendPal;
}
void Sprite::GetBlend(int* blendMode, int* blendPal)
{
    *blendMode = this->blendMode;
    *blendPal = this->blendPal;
}


void Sprite::SetUpdateMethod(void (*updateMethod)(Sprite* hSp))
{
    this->updateMethod = updateMethod;
}
void Sprite::SetDrawingMethod(void (*drawingMethod)(Sprite* hSp, int hX, int hY))
{
    this->drawingMethod = drawingMethod;
}
void Sprite::SetDestructorMethod(void (*destructorMethod)(Sprite* hSp))
{
    this->destructorMethod = destructorMethod;
}

Sprite* Sprite::CopyVisuallyFrom(Sprite* fromSpr)
{
    double x, y, z;
    Graph* image=nullptr;
    int u, v, width, height;
    bool isFlip;
    double rad;
    double scale;
    int blendMode, blendPal;
    Sprite* linkXY=nullptr;

    fromSpr->GetXY(&x, &y);
    z = fromSpr->GetZ();
    fromSpr->GetImage(&image, &u, &v, &width, &height);
    isFlip = fromSpr->GetFlip();
    rad = fromSpr->GetRotationRad();
    scale = fromSpr->GetScale();
    fromSpr->GetBlend(&blendMode, &blendPal);
    linkXY = fromSpr->GetLinkXY();

    Sprite* spr = new Sprite();
    spr->SetXY(x, y);
    spr->SetZ(z);
    spr->SetImage(image, u, v, width, height);
    spr->SetFlip(isFlip);
    spr->SetRotationRad(rad);
    spr->SetScale(scale);
    spr->SetBlend(blendMode, blendPal);
    spr->SetLinkXY(linkXY);
    
    return spr;
}



void Sprite::Destroy(Sprite* spr)
{
    Destroy(spr, false);
}


void Sprite::Destroy(Sprite* spr, bool isRootParentOnly)
{
    if (spr == nullptr) return;

    if (isRootParentOnly)
    {
        if (spr->linkAlive != nullptr) return; // 子なら返る
    }

    if (spr->destructorMethod != nullptr)
    {
        spr->destructorMethod(spr);
    }
    

    spr->sprites[findIndex(spr)] = nullptr;

    if (spr->linkAlive != nullptr)
    {// 親から自分を消す
        auto& childrenOfParent = const_cast<Sprite*>(spr->linkAlive)->linkedChildAlives;
        childrenOfParent.erase(spr);
    }
    for (auto& child : spr->linkedChildAlives)
    {// 子もすべて消す
        if (child == nullptr) continue;
        const_cast<Sprite*>(child)->linkAlive = nullptr; // 親が死んだことを通知
        Destroy(const_cast<Sprite*>(child), true);
    }

}

void Sprite::DestroyAll()
{
    for (Sprite* spr : sprites)
    {
        Destroy(spr, true);
    }
    collectGarbageSprites();
}

int Sprite::findIndex(Sprite* spr)
{
    auto size = sprites.size();
    if (size == 0) return -1;

    auto iter = std::find(sprites.begin(), sprites.end(), spr);
    size_t index = std::distance(sprites.begin(), iter);
    if (index == size)
    {
        return -1;
    }
    return index;
}
void Sprite::collectGarbageSprites()
{
    for (int i = sprites.size()-1; i >= 0; --i)
    {
        if (sprites[i] == nullptr)
        {
            sprites.erase(sprites.begin() + i);
        }
    }
}




void Sprite::UpdateAll()
{
    // auto だとpush_backしたときに謎のメモリバグが発生
    //for (auto spr : sprites)
    //{

    int size = sprites.size();
    for (int i=0; i< size; ++i)
    {
        Sprite* spr = sprites[i];
        if (spr != nullptr)
        {
            if (spr->updateMethod != nullptr) spr->updateMethod(spr);
        }
    }
    collectGarbageSprites();
}

/// <summary>
/// 全Spriteを描画する
/// imageがnullptrのSpriteはdrawingMethodが呼ばれないので注意すること
/// </summary>
void Sprite::DrawingAll()
{
    std::stable_sort(sprites.begin(), sprites.end(),
        [](Sprite* left, Sprite* right) { return left->GetZ() > right->GetZ(); });


    for (Sprite* spr : sprites)
    {
        if (spr != nullptr)
        {
            if (spr->image == nullptr) continue;

            int x, y;
            double x1, y1;
            spr->GetLinkDifferenceXY(&x1, &y1);

            x1 = (spr->x + x1);
            y1 = (spr->y + y1);

            x1 *= PX_PER_GRID;
            y1 *= PX_PER_GRID;

            x = int(x1);
            y = int(y1);

            DxLib::SetDrawBlendMode(spr->blendMode, spr->blendPal);

            if (spr->drawingMethod != nullptr)
            {
                spr->drawingMethod(spr, x, y);
            }
        }
    }
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

}




void Sprite::DrawingKind::AlignGrid(Sprite* hSpr, int hX, int hY)
{
    Draw(hSpr, hX, hY, PX_PER_GRID);
}
void Sprite::DrawingKind::TwoDots(Sprite* hSpr, int hX, int hY)
{
    Draw(hSpr, hX, hY, 2);
}
void Sprite::DrawingKind::DotByDot(Sprite* hSpr, int hX, int hY)
{
    Draw(hSpr, hX, hY, 1);
}
void Sprite::DrawingKind::Draw(Sprite* hSpr, int x, int y, int scale)
{
    double dx = (1.0 - hSpr->scale) * (hSpr->width  / (double(PX_PER_GRID) / scale)) / 2.0;
    double dy = (1.0 - hSpr->scale) * (hSpr->height / (double(PX_PER_GRID) / scale)) / 2.0;

    hSpr->image->DrawGraph(x + dx, y + dy, hSpr->u, hSpr->v, hSpr->width, hSpr->height, scale * hSpr->scale, hSpr->rotationRad, hSpr->isFlip);
}













