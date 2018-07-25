
var component;
var sprite;

var sprite_angle;
var sprite_type;
var sprite_distance;

function createSpriteObjects(type, angle, distence) {
    component = Qt.createComponent("Sprite.qml");
    sprite_angle = angle; sprite_distance = distence; sprite_type = type;
    if (component.status === Component.Ready)
        finishCreation();
    else
        component.statusChanged.connect(finishCreation);
}

function finishCreation() {
    console.log(sprite_angle + " " + sprite_distance)
    if (component.status === Component.Ready) {
        sprite = component.createObject(root, {"angle": sprite_angle, "sprite_distance": sprite_distance,"spriteType":sprite_type});
        if (sprite === null) {
            // Error Handling
            console.log("Error creating object");
        }
    } else if (component.status === Component.Error) {
        // Error Handling
        console.log("Error loading component:", component.errorString());
    }
    return sprite
}
