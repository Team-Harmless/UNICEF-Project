
function createSpriteObjects(type, angle, distance, id) {
    var component = Qt.createComponent("Lines.qml");
    var sprite = component.createObject(root, {"angle": angle, "distance": distance,"spriteType":type, "id":id});

    if (sprite === null) {
        // Error Handling
        console.log("Error creating object");
    }

    component = Qt.createComponent("Sprite.qml");
    sprite = component.createObject(root, {"angle": angle, "distance": distance,"spriteType":type, "id":id});
    if (sprite === null) {
        // Error Handling
        console.log("Error creating object");
    }
}

function kmToPx(km, planeKM, planeDim) {
    return planeDim / planeKM * km
}
