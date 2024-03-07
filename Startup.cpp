#include "Game.h"
#include "Components/CubeComponent.h"
#include "Components/SquareComponent.h"
#include "Controllers/CameraController3D.h"
#include "Controllers/CameraControllerRadial.h"
#include "Controllers/PlanetController.h"
#include "Objects/CameraObject.h"
#include "Objects/PlanetObject.h"

int main()
{
    auto* game = Game::Instance();
    constexpr auto size = Point{800, 800};

    auto* camera_object = new objects::CameraObject();
    camera_object->Compose(size);

    game->Compose(L"Game", size, 0.1f, camera_object->camera());

    auto* camera_controller_3d = new controllers::CameraController3D();
    camera_controller_3d->Compose(camera_object, game->input_device());
    game->variable_update()->Add(camera_controller_3d);

    auto* camera_controller_radial = new controllers::CameraControllerRadial();
    camera_controller_radial->Compose(camera_object, game->input_device());
    camera_controller_radial->Compose(game->physics_collision());
    game->variable_update()->Add(camera_controller_radial);

    camera_controller_3d->Compose(camera_controller_radial);
    camera_controller_radial->Compose(camera_controller_3d);
    camera_controller_3d->is_active() = true;

    auto* sun = new objects::PlanetObject();
    sun->Compose(float3(0, 0, -2), float3(2), new components::CubeComponent());
    auto* square_controller1 = new controllers::PlanetController();
    square_controller1->Compose(sun, nullptr);
    square_controller1->SetSpeed(float3(), float3(), 0,
                                 float3(0, 1, 0), 2);

    auto* mercury = new objects::PlanetObject();
    mercury->Compose(float3(-2, 0, -2), float3(0.6f), new components::CubeComponent());
    auto* square_controller2 = new controllers::PlanetController();
    square_controller2->Compose(mercury, sun->transform());
    square_controller2->SetSpeed(
        mercury->transform()->position() - sun->transform()->position(),
        float3(0, 1, 0), 2,
        float3(0, 1, 0), 1.5f);

    auto* venus = new objects::PlanetObject();
    venus->Compose(float3(0, 0, 2), float3(1), new components::CubeComponent());
    auto* square_controller3 = new controllers::PlanetController();
    square_controller3->Compose(venus, sun->transform());
    square_controller3->SetSpeed(
        venus->transform()->position() - sun->transform()->position(),
        float3(1, 1, 0), 1.5f,
        float3(0, 1, 0), -2);

    auto* earth = new objects::PlanetObject();
    earth->Compose(float3(0, 0, -5), float3(1), new components::CubeComponent());
    auto* square_controller4 = new controllers::PlanetController();
    square_controller4->Compose(earth, sun->transform());
    square_controller4->SetSpeed(
        earth->transform()->position() - sun->transform()->position(),
        float3(-1, 1, 0), -1.5f,
        float3(1, 1, 0), 2);

    auto* lune = new objects::PlanetObject();
    lune->Compose(float3(0.5f, 0, -5), float3(0.4f), new components::CubeComponent());
    auto* square_controller4_1 = new controllers::PlanetController();
    square_controller4_1->Compose(lune, earth->transform());
    square_controller4_1->SetSpeed(
        lune->transform()->position() - earth->transform()->position(),
        float3(0, 1, 0), 3,
        float3(1, 0, 0), 1);

    auto* mars = new objects::PlanetObject();
    mars->Compose(float3(0, 0, 4), float3(2), new components::CubeComponent());
    auto* square_controller5 = new controllers::PlanetController();
    square_controller5->Compose(mars, sun->transform());
    square_controller5->SetSpeed(
        mars->transform()->position() - sun->transform()->position(),
        float3(1, 0, 0), 0.5f,
        float3(1, 0, 0), 1);

    auto* fobos = new objects::PlanetObject();
    fobos->Compose(float3(-0.5, 0, 4.5), float3(0.3f), new components::CubeComponent());
    auto* square_controller5_1 = new controllers::PlanetController();
    square_controller5_1->Compose(fobos, mars->transform());
    square_controller5_1->SetSpeed(
        fobos->transform()->position() - mars->transform()->position(),
        float3(1, 0, 1), 2,
        float3(1, 0, 1), 1);

    auto* demos = new objects::PlanetObject();
    demos->Compose(float3(0, 1, 3), float3(0.2f), new components::CubeComponent());
    auto* square_controller5_2 = new controllers::PlanetController();
    square_controller5_2->Compose(demos, mars->transform());
    square_controller5_2->SetSpeed(
        demos->transform()->position() - mars->transform()->position(),
        float3(0, 1, 1), 2.5f,
        float3(0, 0, 1), -2);

    game->render_pipeline()->Add(sun->render());
    game->render_pipeline()->Add(mercury->render());
    game->render_pipeline()->Add(venus->render());
    game->render_pipeline()->Add(earth->render());
    game->render_pipeline()->Add(lune->render());
    game->render_pipeline()->Add(mars->render());
    game->render_pipeline()->Add(fobos->render());
    game->render_pipeline()->Add(demos->render());
    game->variable_update()->Add(square_controller1);
    game->variable_update()->Add(square_controller2);
    game->variable_update()->Add(square_controller3);
    game->variable_update()->Add(square_controller4);
    game->variable_update()->Add(square_controller4_1);
    game->variable_update()->Add(square_controller5);
    game->variable_update()->Add(square_controller5_1);
    game->variable_update()->Add(square_controller5_2);
    game->physics_collision()->Add(sun);
    game->physics_collision()->Add(mercury);
    game->physics_collision()->Add(venus);
    game->physics_collision()->Add(earth);
    game->physics_collision()->Add(lune);
    game->physics_collision()->Add(mars);
    game->physics_collision()->Add(fobos);
    game->physics_collision()->Add(demos);
    game->Run();
}
