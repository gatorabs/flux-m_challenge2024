import { firebaseRef5, firebaseRef6 } from "./config_firebase.js";
import { setRotationX, setRotationY } from "./model_3d.js"
export function listen_gyro(){
    var gyroX;
    var gyroY;
    firebaseRef5.on('value', function (snapshot) {
      gyroX = snapshot.val();
      let radiansX = gyroX * Math.PI / 180;

      setRotationX(radiansX);
      
      renderer.render(scene, camera);
  
    });
    firebaseRef6.on('value', function (snapshot) {
      gyroY = snapshot.val();
      let radiansY = gyroY * Math.PI / 180;

      setRotationY(radiansY);
      
      renderer.render(scene, camera);
  
    });
   

}
