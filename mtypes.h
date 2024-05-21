#ifndef MTYPES_H
#define MTYPES_H

#define PC_SCALE    4   // Коэффициент масштабирования экрана компьютера относительно реального размера дисплея

typedef enum
{
	GuiIconSet,
	GuiLabel,
	GuiMenu,
	GuiProgress,
	GuiCombo,
	GuiNone
} GuiType;

typedef enum
{
    PropString,
    PropNumber,
    PropCombo,
    PropFiles,
    PropList
} PropType;

#endif // MTYPES_H
