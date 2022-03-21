-- class IngameEventBase
IngameEventBase = {
    new = function()
        local self = Instantiate(IngameEventBase)
        
        self.time = 0
        self.events = {}
        self.pausedEvents = {}
        
        return self
    end,

    -- Do update corutines id self.events[key].
    -- You can pause corutines if the coroutine have self.pausedEvents[key].
    update = function(self)
        for key, value in pairs(self.events) do
            if (coroutine.status(value))~="dead" then
                if (self.pausedEvents[key]~=true) then
                    local ok, res = coroutine.resume(value)
                    if (not ok) then
                        ErrLog("Corutine erroed at "..key.." in "..debug.getinfo(2).source.."\n"..res.."\n")
                    end
                end
            else
                self.events[key] = nil
            end
        end

        self.time = self.time+Time.deltaMilli()
        return 0
    end,

    doSleep = function (time)
        local t = 0
        while t<time do 
            t = t + Time.deltaSec()
            Yield(true)
        end
        return false
    end,

    eventDrive = function (self, key, e)
        if (self[key]==nil) then
            ErrLog("Event function: "..key.." is not exit.\n");
            return
        elseif (self.events[key]~=nil) then
            ErrLog("Event function: "..key.." is already running.\n")
            return
        end

        OutLog("Event: "..key.." is driven.\n")

        self.events[key] = coroutine.create(self[key])
        coroutine.resume(self.events[key], self, e)
    end,

    awaits = {
        sleep = function (time)
            local t = 0
            while t<time do 
                t = t + Time.deltaSec()
                Yield()
            end
        end,

        streamText = function (m, str)
            m:streamText(str)
            while m:isRunning() do Yield() end
        end,

        selectionWindow = function (table)
            local s = SelectionWindow.open(table)
            while s:isRunning() do Yield() end
            local index=s:selectedIndex()
            local word = s:selectedWord()
            s:close()
            return index, word
        end
    }

}

