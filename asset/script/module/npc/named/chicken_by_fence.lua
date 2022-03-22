

chicken_by_fence = {

    new = function()
        
        local self = Instantiate(chicken_by_fence, ChickenLuaData)
        
        self.events = {
            move = nil,
        }
        self.vel = 30

        return self
    end,

    update = function(self)
        ChickenLuaData.update(self)

        if (self.events.move==nil) then
            self.events.move = coroutine.create(self.move)
            coroutine.resume(self.events.move, self)
        end

    end,

    move = function (self)
        if not FlagManager.getFlag(FlagName.send_all_chick) then
            for i = 1, 4, 1 do
                local e = MapEventManager.getUnique("chicken_by_fence_move_"..i)
                while self.doMove(e.x, e.y) do Yield() end
                if i==2 then
                    local c = coroutine.create( self.doSleep )
                    while coroutine.resume(c, 1.5) do Yield() end
                end
            end
        else
            for i = 1, 8, 1 do
                local e = MapEventManager.getUnique("chicken_by_fence_move_a_"..i)
                while self.doMove(e.x, e.y) do Yield() end
                if i==2 or i==4 then
                    local c = coroutine.create( self.doSleep )
                    while coroutine.resume(c, 1.5) do Yield() end
                end
            end
            local e = MapEventManager.getUnique("chicken_by_fence_move_a_2")
            while self.doMove(e.x, e.y) do Yield() end
        end
    end,

    talk = function (self, e)
        local w = MessageWindow.open()

        if not FlagManager.getFlag(FlagName.pop_chick_1st) then
            w:streamText([[5羽の子どもたちが迷子になってしまって]].."\n"..[[困ったわ..]])
            while w:isRunning() do Yield() end

            local e1 = MapEventManager.getUnique("chick_stray_1")
            MapEventManager.installCharacter(e1.x, e1.y, "chick", "chick_1")

            local e2 = MapEventManager.getUnique("chick_stray_2")
            MapEventManager.installCharacter(e2.x, e2.y, "chick", "chick_2")

            local e3 = MapEventManager.getUnique("chick_stray_3")
            MapEventManager.installCharacter(e3.x, e3.y, "chick", "chick_3")
            
            FlagManager.setFlag(FlagName.pop_chick_1st, true)
        elseif not FlagManager.getFlag(FlagName.send_all_chick) then
            local catchCount = 0

            local counter = function (flagname)
                if FlagManager.getFlag(flagname) then catchCount = catchCount+1 end    
            end

            counter(FlagName.catch_chick_1)
            counter(FlagName.catch_chick_2)
            counter(FlagName.catch_chick_3)
            counter(FlagName.catch_chick_4)
            counter(FlagName.catch_chick_5)

            if catchCount==0 then
                w:streamText([[どこに行ってしまったのかしら..]])
                while w:isRunning() do Yield() end
            elseif catchCount==5 then
                w:streamText([[おや! 子供たちがそろってますね]])
                while w:isRunning() do Yield() end

                w:streamText("\n"..[[届けてくださって]].."\n"..[[本当にありがとうございました!]])
                while w:isRunning() do Yield() end

                w:close()
                w = MessageWindow.open()
            
                w:streamText([[おねえちゃん、ありがとう!]])
                while w:isRunning() do Yield() end

                w:close()
                w = MessageWindow.open()
            
                w:streamText([[また遊んでね!!]])
                while w:isRunning() do Yield() end

                FlagManager.setFlag(FlagName.send_all_chick, true)
            else
                w:streamText([[ああ、子どもを連れてきてくださり]].."\n"..[[ありがとうございます!]])
                while w:isRunning() do Yield() end

                w:streamText("\n"..[[でもすいません、あと]]..catchCount..[[匹]].."\n"..[[見つけてほしいです]])
                while w:isRunning() do Yield() end
            end
        else
            w:streamText([[この度は本当にありがとうございました!]])
            while w:isRunning() do Yield() end
        end

        w:close()
    end
    
}








