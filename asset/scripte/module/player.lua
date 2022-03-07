




-- class Player
Player = {

    new = function(actor, startX, startY)
        

        local self = {}
        setmetatable(self, {__index=Player})
        
        self.actor = actor
        self.spr = actor:getSpr()
        self.x = startX
        self.y = startY
        self.time = 0
      
        self.spr:setXY(self.x, self.y)
        self.spr:setImage(Images.Kisaragi)
        
        return self
    end,

    update = function(self)
        Cout(" "..self.x)
        --self.x = self.x + 0.5
        self.x = self.x + 8 * Time.deltaSec()
        self.y = self.y - 8 * Time.deltaSec()
        self.time = self.time+Time.deltaMilli()
        self.spr:setXY(self.x, self.y)

        if (self.time>100) then
            self.doMove(20, 40)    
        end

        return 0
    end,


}








