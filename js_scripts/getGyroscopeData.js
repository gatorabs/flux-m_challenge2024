import { firebaseRef5, firebaseRef6, firebaseRef7 } from "./config_firebase.js";
import { setRotationX, setRotationY, setRotationZ } from "./model_3d.js"
export function listen_gyro(){
    var gyroX;
    var gyroY;
    var gyroZ;
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
    firebaseRef7.on('value', function (snapshot) {
      gyroZ = snapshot.val();
      let radiansZ = gyroZ * Math.PI / 180;

      setRotationZ(radiansZ);
      
      renderer.render(scene, camera);
  
    });
   

}
