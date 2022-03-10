MapEventManager:addMapEvents({

    house_1 = function (self, e)
        OutLog("Enter house.\n")
        local c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end
        OutLog("Ok.\n")
    end,

    house_2 = function (self, e)
        local w = MessageWindow.open()

        w:streamText("‚±‚ê‚Í‰Æ‚Å‚·\n“ú–{Œê‚í‚¢‚í‚¢.\n")
        while w:isRunning() do Yield() end

        --w:streamText("Here is House 2.")
        --while w:isRunning() do Yield() end

        OutLog("‚±‚ê‚Í‰Æ‚Å‚·\n")
        local c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end


        OutLog("Ok.\n")
        w:close()
    end
})


