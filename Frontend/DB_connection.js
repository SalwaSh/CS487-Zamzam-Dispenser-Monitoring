// Firebase configuration
const firebaseConfig = {
    
};

// Initialize Firebase and declare the database variable
firebase.initializeApp(firebaseConfig);
var database = firebase.database();

// Fetch data from Firebase
function fetchData() {
    // Get reference to the 'Containers' node in the database
    var dataRef = database.ref('Containers');
    // Listen for value changes in the 'Containers' node
    dataRef.on('value', function(snapshot) {
        document.getElementById('data-container-wrapper').innerHTML = "";
        // Iterate over each child snapshot
        snapshot.forEach(function(childSnapshot) {
            var containerData = childSnapshot.val();
            createDataContainer(containerData); // Create a data container for the containerData
            createCircularChart(containerData); // Create a circular chart for the containerData
        });
    }, function(error) {
        console.error('Error fetching data:', error);
    });
}

// Create a new data container div
function createDataContainer(containerData) {
    // Create a new data container div
    var newDataContainer = document.createElement('div');
    newDataContainer.className = 'data-container';

    // Create data item for Container ID
    var idItem = document.createElement('div');
    idItem.className = 'data-item';
    idItem.innerHTML = '<h2>Container ID</h2><p class="value" id="ID">' + containerData.ID + '</p>';
    newDataContainer.appendChild(idItem);

    // Create data item for Water Level
    var waterLevelItem = document.createElement('div');
    waterLevelItem.className = 'data-item';
    waterLevelItem.innerHTML = '<h2>Water Level</h2><p class="value" id="water_value">' +  parseInt((9.792 - (containerData.waterLevel))/9.792*100) + ' %</p>';
    newDataContainer.appendChild(waterLevelItem);

    // Create chart container for water level chart
    var waterChartItem = document.createElement('div');
    waterChartItem.className = 'chart-container';
    waterChartItem.innerHTML = '<canvas class="chart" id="waterChart' + containerData.ID + '" width="200" height="200"></canvas>';
    newDataContainer.appendChild(waterChartItem);

    // Append the new data container to the main container
    document.getElementById('data-container-wrapper').appendChild(newDataContainer);
}

// Create circular chart for water level
function createCircularChart(containerData) {
    var gradientColors = (containerData.waterLevel >= 4.5) ? ['red', '#F5F5F5'] : ['green', '#F5F5F5'];

    var ctx = document.getElementById('waterChart' + containerData.ID).getContext('2d');
    var waterChart = new Chart(ctx, {
        type: 'doughnut',
        data: {
            datasets: [{
                data: [parseInt((9.792 - (containerData.waterLevel))/9.792*100), 100 - parseInt((9.792 - (containerData.waterLevel))/9.792*100)], //The first element is the waterLevel value, and the second element is the remaining space
                backgroundColor: gradientColors, // Set the background color using the gradientColors based on the condition.
                borderColor: gradientColors, // Set the border Color using the gradientColors based on the condition.
                borderWidth: 2 //The borderWidth is set to 2 pixels.
            }]
        },
        options: {
            cutout: '80%', // Set the size of the center hole in the doughnut chart
            responsive: true, // Make the chart responsive to resize
            maintainAspectRatio: false, // Do not maintain aspect ratio between the height and width
        }
    });
}

// fetch data
fetchData();
