package fr.adriencournand.stock

import fr.adriencournand.stock.user_interface.gui.GuiBuilder
import fr.adriencournand.stock.user_interface.gui.GuiType

fun main() {
    GuiBuilder.guiFrom(GuiType.JAVAFX).start()
}