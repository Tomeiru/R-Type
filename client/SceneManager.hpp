namespace RType::Client {
/**
 * @brief ClientSceneManager is a class that manages the scenes of the client
 */
class SceneManager {
public:
    /**
     * @brief Enum that represents the scenes of the client
     */
    enum Scene {
        MAIN_MENU,
        LOBBY,
        GAME,
        ALL,
        QUIT
    };

    /**
     * @brief Construct a new ClientSceneManager object
     */
    SceneManager()
        : current_scene(MAIN_MENU)
    {
    }

    /**
     * @brief Set the current scene
     *
     * @param scene Current scene
     */
    void setCurrentScene(Scene scene)
    {
        current_scene = scene;
    }

    /**
     * @brief Get the current scene
     *
     * @return Scene
     */
    Scene getCurrentScene() const
    {
        return current_scene;
    }

private:
    Scene current_scene;
};
}