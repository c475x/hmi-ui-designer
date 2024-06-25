#ifndef MTYPES_H
#define MTYPES_H

#define PC_SCALE 4 // Коэффициент масштабирования экрана компьютера относительно реального размера дисплея

// Типы графических элементов
typedef enum
{
	GuiIconSet,
	GuiLabel,
	GuiMenu,
	GuiProgress,
	GuiCombo,
	GuiNone
} GuiType;

// Типы свойств графических элементов
typedef enum
{
	PropString,
	PropNumber,
	PropFiles,
	PropList,
} PropType;

#endif // MTYPES_H
