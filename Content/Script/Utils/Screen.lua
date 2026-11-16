local Module = {}

local UEPrintString = UE.UKismetSystemLibrary.PrintString

function Module.Print(text, color, duration)
	color = color or UE.FLinearColor(0, 1, 0, 1)
	duration = duration or 10
	print(text)
	UEPrintString(nil, text, true, false, color, duration)
end

return Module
