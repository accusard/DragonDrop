//
//  hitbox_extenstion.h
//  dragon_drop
//
//  Created by Vanny Sou
//
//

#ifndef dragon_drop_hitbox_extenstion_h
#define dragon_drop_hitbox_extenstion_h

class HitBoxExtension
{
public:
    CIwGameDummySprite* InitHitBox(CIwGameUIBase* ui, int ext_width, int ext_height)
    {
        //Init the hitbox of the bar icon
        CIwGameBrushImage* brush = (CIwGameBrushImage*)ui->getBackgroundBrush();
        
        if(brush != NULL)
        {
            CIwGameDummySprite* sprite = new CIwGameDummySprite();
            int width  = brush->getSrcRect().w + ext_width;
            int height  = brush->getSrcRect().h + ext_height;
            
            sprite->Init();
            sprite->setDestSize(width, height);
            sprite->setPosition( ui->getPosition().x, ui->getPosition().y);
            CIwGameSprite* parent_sprite = (CIwGameSprite*)ui->getVisual()->getParent();
            if(parent_sprite != NULL)
                parent_sprite->addChild(sprite);
            
            ui->getScene()->getSpriteManager()->addSprite(sprite);
            
            return sprite;
        }
        return NULL;
    }
    
};

#endif
