MapEventManager:addMapEvents({

    house_2 = function (self, e)
        local m = MessageWindow.open()

        if FlagManager.getFlag(FlagName.friend_with_mouse) then
            m:streamText([[あなた、ネズミを食べなかったのね]])
            while m:isRunning() do Yield() end

            m:streamText("\n"..[[優しいわね..]])
            while m:isRunning() do Yield() end
        elseif FlagManager.getFlag(FlagName.obtain_super_dash) then
            m:streamText([[ネズミ、食べたのね]])
            while m:isRunning() do Yield() end
            m:streamText("\n"..[[羨ましいわ]])
            while m:isRunning() do Yield() end
        else
            m:streamText([[お城に向かっていく]].."\n"..[[ネズミを見たことがあるわ]])
            while m:isRunning() do Yield() end
    
            m:streamText("\n"..[[きっとおいしいに決まってるわ]])
            while m:isRunning() do Yield() end
    
            m:streamText("\n"..[[根拠はないけど]])
            while m:isRunning() do Yield() end

            FlagManager.setFlag(FlagName.heard_mouse_rumor, true)
        end

        m:close()

    end
})
