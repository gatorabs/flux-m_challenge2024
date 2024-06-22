import { firebaseRef5 } from "./config_firebase.js";
export function listen_gyro(){
    var gyroX;
    firebaseRef5.on('value', function (snapshot) {
      gyroX = snapshot.val();
      cube.rotation.x = gyroX
  
      renderer.render(scene, camera);
  
    });
}

let scene, camera, renderer, cube;

function parentWidth(elem) {
  return elem.parentElement.clientWidth;
}
function parentHeight(elem) {
  return elem.parentElement.clientHeight;
}
export function init3D() {
  scene = new THREE.Scene();
  scene.background = new THREE.Color(0x3F3F3F);

  camera = new THREE.PerspectiveCamera(75, parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube")), 0.1, 1000);

  renderer = new THREE.WebGLRenderer({ antialias: true });
  renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));

  document.getElementById('3Dcube').appendChild(renderer.domElement);


  const geometry = new THREE.BoxGeometry(3, 1, 2);


  var cubeMaterials = [
    new THREE.MeshBasicMaterial({ color: 0x03045e }),
    new THREE.MeshBasicMaterial({ color: 0x023e8a }),
    new THREE.MeshBasicMaterial({ color: 0x0077b6 }),
    new THREE.MeshBasicMaterial({ color: 0x03045e }),
    new THREE.MeshBasicMaterial({ color: 0x023e8a }),
    new THREE.MeshBasicMaterial({ color: 0x0077b6 }),
  ];

  const material = new THREE.MeshFaceMaterial(cubeMaterials);

  cube = new THREE.Mesh(geometry, material);
  scene.add(cube);
  camera.position.z = 5;

  //listener HERE
  cube.rotation.x = 0

  renderer.render(scene, camera);
}