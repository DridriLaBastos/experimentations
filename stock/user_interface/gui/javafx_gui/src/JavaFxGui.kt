package fr.adriencournand.stock.user_interface.gui.javafx_gui

import javafx.application.Application
import javafx.scene.Scene
import javafx.stage.Stage

class JavaFxGui:Application(), fr.adriencournand.stock.user_interface.IUserInterface {
    override fun start() {
        launch()
    }

    override fun start(primaryStage: Stage?) {
        primaryStage?.title = "Stock JavaFX"
        val root = javafx.scene.layout.StackPane()
        root.children.add(javafx.scene.control.Label("Hello JavaFx World !"))

        primaryStage?.scene = Scene(root)
        primaryStage?.show()
    }

}