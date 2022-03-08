

-- class Punicat
Punicat = {

    new = function()
        
        local self = Instantiate(Punicat, IngameEventBase)
        
        self.vel = 100
        self.frameInterval = 200

        return self
    end,

    update = function(self)
        IngameEventBase.update(self)
    end,
}








