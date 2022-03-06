

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
        print("x="..self.x)
        self.spr:setXY(self.x, self.y)
        self.spr:setImage(Images.Kisaragi)
        
        return self
    end,

    update = function(self)
        Cout(" "..self.x)
        --self.x = self.x + 0.5
        self.x = self.x + 0.2
        self.y = self.y + 0.1
        self.time = self.time+1
        self.spr:setXY(self.x, self.y)
        
        
        

        return 0
    end,
}








