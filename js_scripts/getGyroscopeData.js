import { firebaseRef5 } from "./config_firebase.js";
import { setRotationX } from "./model_3d.js"
export function listen_gyro(){
    var gyroX;
    firebaseRef5.on('value', function (snapshot) {
      gyroX = snapshot.val();
      let radiansX = gyroX * Math.PI / 180;

      setRotationX(radiansX);
      
      renderer.render(scene, camera);
  
    });
}
