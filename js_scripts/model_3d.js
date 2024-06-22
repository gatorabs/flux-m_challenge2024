let scene, camera, renderer, object;
let rotationX = 0;
let rotationY = 0;

function init() {
  const container = document.getElementById('3Dcube');

  
  scene = new THREE.Scene();

  const aspectRatio = 481 / 458;
  camera = new THREE.PerspectiveCamera(75, aspectRatio, 0.1, 1000);
  camera.position.set(0, 0, 20);

  
  renderer = new THREE.WebGLRenderer({ antialias: true });
  renderer.setSize(481, 458);
  renderer.setClearColor(0x2C2C2C);
  container.appendChild(renderer.domElement);

  
  const ambientLight = new THREE.AmbientLight(0xffffff, 0.5); 
  scene.add(ambientLight);

  const directionalLight = new THREE.DirectionalLight(0xffffff, 1);
  directionalLight.position.set(1, 1, 1).normalize();
  scene.add(directionalLight);

  // Carregar modelo STL
  const loader = new THREE.STLLoader();
  loader.load('./AU20_Scania2.stl', function (geometry) {
    console.log('STL carregado com sucesso');
    geometry.computeVertexNormals();
    
    
    const material = new THREE.MeshPhongMaterial({
      color: 0x00ffff,
      shininess: 100,
      specular: 0x111111,
      side: THREE.DoubleSide 
    });

    
    object = new THREE.Mesh(geometry, material);
    object.position.set(0, 1.5, 0);
    object.scale.set(0.1, 0.1, 0.1);
    scene.add(object);
    console.log('Objeto adicionado à cena:', object);
  }, undefined, function (error) {
    console.error('Erro ao carregar o arquivo STL:', error);
  });

  
  window.addEventListener('resize', onWindowResize, false);
}


function onWindowResize() {
  const container = document.getElementById('3Dcube');
  const aspectRatio = 481 / 458;
  camera.aspect = aspectRatio;
  camera.updateProjectionMatrix();
  renderer.setSize(481, 458);
}


function animate() {
  requestAnimationFrame(animate);
  

  if (object) {
    object.rotation.x = rotationX;
    object.rotation.y = rotationY;
  }

  
  renderer.render(scene, camera);
}


init();
animate();


export function setRotationX(value) {
  rotationX = value;
}


export function setRotationY(value) {
  rotationY = value;
}
