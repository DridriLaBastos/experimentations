package fr.adriencournand.stock.user_interface.gui

import fr.adriencournand.stock.user_interface.IUserInterface
import fr.adriencournand.stock.user_interface.gui.javafx_gui.JavaFxGui

class GuiBuilder {

    companion object
    {
        fun guiFrom(type: GuiType) =
            when (type)
            {
                GuiType.JAVAFX -> JavaFxGui()
            }

    }
}