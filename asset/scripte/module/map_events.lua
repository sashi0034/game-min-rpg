MapEventManager:addMapEvents({

    house_1 = function (self, e)
        OutLog("Enter house.\n")
        local c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end
        OutLog("Ok.\n")
    end,

    house_2 = function (self, e)
        local m = MessageWindow.open()

        m:streamText("Ç®ÇÕÇÊÇ§Ç≤Ç¥Ç¢Ç‹Ç∑")
        while m:isRunning() do Yield() end

        local s = SelectionWindow.open({"Ç±ÇÒÇ…ÇøÇÕ", "Ç®ÇÕÇÊÇ§", "ñ∞Ç¢", "Ç¢Ç¢Ç¶", "Ç†ÇËÇ™Ç∆Ç§"})
        while s:isRunning() do Yield() end
        local index=s:selectedIndex()
        s:close()

        if index==0 then
            m:streamText("\n"..[[Ç±ÇÒÇ…ÇøÇÕÇ±ÇÒÇ…ÇøÇÕ]])
            while m:isRunning() do Yield() end
        elseif index==1 then
            m:streamText("\n"..[[Ç®ÇÕÇÊÇ§Ç≤Ç¥Ç¢Ç‹Ç∑ÅAÇ®ÇÕÇÊÇ§Ç≤Ç¥Ç¢Ç‹Ç∑ÅAÇ®ÇÕÇÊÇ§Ç≤Ç¥Ç¢Ç‹Ç∑]])
            while m:isRunning() do Yield() end
        elseif index==2 then
            m:streamText("\n"..[[ñ∞Ç¢Ç≈Ç∑ÇÀ]])
            while m:isRunning() do Yield() end
        elseif index==3 then
            m:streamText("\n"..[[Ç¢Ç¢Ç¶Ç±ÇøÇÁÇ±Çª]].."\n"..[[Ç»Ç…Ç±ÇÍ??]])
            while m:isRunning() do Yield() end
        elseif index==4 then
            m:streamText("\n"..[[ä¥é”ÇÃãCéùÇøÇéùÇøÇ‹ÇµÇÂÇ§]])
            while m:isRunning() do Yield() end
        end

        m:close()
    end,

    
    stones_1 = function (self, e)
        local w = MessageWindow.open()

        w:streamText("êŒÇ±ÇÎÇ™ì]Ç™Ç¡ÇƒÇ¢ÇÈ")
        while w:isRunning() do Yield() end
        
        local s = SelectionWindow.open({"ÇÕÇ¢", "Ç¢Ç¢Ç¶"})
        while s:isRunning() do Yield() end
        s:close()

        Yield()
        
        w:close()

        w = MessageWindow.open()
        w:streamText("ñ∞Ç¢\nÇÀÇﬁÇ¢Ç≈Ç∑\nÇÀÇ‹Ç∑")
        while w:isRunning() do Yield() end

        local c = coroutine.create( self.doSleep )
        while coroutine.resume(c, 2.5) do Yield() end

        w:close()
    end,

    tree_1 = function (self, e)
        local w = MessageWindow.open()

        w:streamText("ïÅí ÇÃñÿÇ™Ç†ÇÈ")
        while w:isRunning() do Yield() end
   
        w:close()
    end
})


