
function createSpriteObjects(type, angle, distance, id, index, clickable) {
    var component = Qt.createComponent("Lines.qml");
    var sprite = component.createObject(root, {"angle": angle, "distance": distance,"spriteType":type, "id":id, "clickable":clickable, "arrayIndex": index});

    if (sprite === null) {
        // Error Handling
        console.log("Error creating object");
    }

    component = Qt.createComponent("Sprite.qml");
    sprite = component.createObject(root, {"angle": angle, "distance": distance,"spriteType":type, "title":id, "arrayIndex": index, "browserfy": clickable});
    if (sprite === null) {
        // Error Handling
        console.log("Error creating object");
    }
}

function kmToPx(km, planeKM, planeDim) {
    return planeDim / planeKM * km
}
