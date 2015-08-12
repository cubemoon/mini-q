
local SampleScene = class("SampleScene", function()
    return display.newScene("SampleScene")
end)

function SampleScene:ctor()
    self.samples = dofile(__G__QUICK_PATH__ .. "/samples/samples.lua")
    local bg     = display.newColorLayer(ccc4(255, 255, 255, 255))
    self:addChild(bg)

    self:createLogo()
    self:createPageView()
    self:createCopyright()
end

function SampleScene:createLogo()
    cc.ui.UIPushButton("LogoBanner.png")
        :onButtonClicked(function()
            CCNotificationCenter:sharedNotificationCenter():postNotification("WELCOME_APP")
        end)
        :align(display.LEFT_TOP, display.left + 20, display.top - 4)
        :addTo(self, 1)
end

function SampleScene:createCopyright()
    local label = ui.newTTFLabel({
        text = "Copyright (c) 2012-2014 chukong-inc.com, licensed under MIT.",
        size = 14,
        color = ccc3(144, 144, 144),
        x = display.cx,
        y = display.bottom + 24,
        align = ui.TEXT_ALIGN_CENTER,
    })
    self:addChild(label)
end

function SampleScene:createPageView()

    local originLeft  = display.left + 130
    local left        = originLeft
    local originTop   = display.top - 180
    local top         = originTop

    local hMargin     = 30

    local imageWidth  = 180
    local imageHeight = 150

    local sampleCount = #self.samples
    local maxNum      = 12

    local pageCount = math.ceil(sampleCount / maxNum)

    self.pv = cc.ui.UIPageView {
            viewRect = cc.rect(40, 40, display.width - 80, display.height - 50),
            column = 4, row = 3,
            padding = {left = 0, right = 20, top = 20, bottom = 0},
            columnSpace = 30, rowSapce = 10}
        :onTouch(function ( event )
            if event.name == cc.ui.UIPageView.EVT_CLICKED then
                local sampleIndex = 12 * (event.pageIdx - 1) + event.itemIdx
                self:onSampleClicked(self.samples[sampleIndex])
            end
        end)
        :addTo(self)

    local sampleIndex = 1
    for pageCount = 1, pageCount do
        for i = 1, 3 do
            for j = 1, 4 do
                local sample = self.samples[sampleIndex]
                sampleIndex = sampleIndex + 1

                if sample ~= nil then
                    local item = self.pv:newItem()
                    local content = display.newNode()

                    cc.ui.UILabel({text = sample.description, size = 12, color = ccc3(50,144,144)})
                        :align(display.CENTER)
                        :pos(imageWidth/2, 150)
                        :addTo(content)

                    display.newSprite(sample.image or "ListSamplesButton_zh.png")
                        :pos(imageWidth/2, 140/2)
                        :addTo(content)

                    item:addChild(content)
                    self.pv:addItem(item)
                else
                    break
                end
            end
        end

    end

    self.pv:reload()
end

function SampleScene:onSampleClicked(sample)
    local configPath = __G__QUICK_PATH__ .. sample.path .. "/scripts/config.lua"
    dofile(configPath)
    local args = {
        "-workdir",
        __G__QUICK_PATH__ .. sample.path,
        "-size",
        CONFIG_SCREEN_WIDTH.."x"..CONFIG_SCREEN_HEIGHT,
        "-" .. CONFIG_SCREEN_ORIENTATION,
    }
    local projectPath = __G__QUICK_PATH__ .. sample.path
    local commandline = "-workdir," .. projectPath .. ",-size," .. CONFIG_SCREEN_WIDTH.."x"..CONFIG_SCREEN_HEIGHT .. ",-" .. CONFIG_SCREEN_ORIENTATION
    self.projectArgs = CCString:create(commandline) --CCString:create(json.encode(args))
    CCNotificationCenter:sharedNotificationCenter():postNotification("WELCOME_OPEN_PROJECT_ARGS",self.projectArgs)
end

return SampleScene
