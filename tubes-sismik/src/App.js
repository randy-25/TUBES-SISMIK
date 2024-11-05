import React, { useState, useEffect, useRef } from 'react';
import axios from 'axios';

const App = () => {
  const [speed, setSpeed] = useState(null);
  const esp32Url = "http://192.168.18.74";
  const offIntervalRef = useRef(null);

  const carControl = (state) => {
    axios.get(`/control?state=${state}`)
      .then(response => console.log(response.data))
      .catch(error => console.error(error));

    if (offIntervalRef.current) {
      clearInterval(offIntervalRef.current);
    }
    // Set a new interval to turn off after 2 seconds
    offIntervalRef.current = setTimeout(() => {
      carControl('off');
    }, 2000);
  }

  const fetchSpeed = () => {
    axios.get(`/speed`)
      .then(response => {
        const speed = response.data.speed;
        setSpeed(speed === -1 ? "Can't Get Speed" : `${speed.toFixed(2)} cm/s`);
      })
      .catch(error => console.error("Failed to fetch speed: ", error));
  };

  useEffect(() => {
    // Fetch speed every 500 ms
    const interval = setInterval(fetchSpeed, 500);
    return () => clearInterval(interval);
  }, []); 

  useEffect(() => {
    return () => {
      if (offIntervalRef.current) {
        clearInterval(offIntervalRef.current);
      }
    };
  }, []);

  return (
    <div class="flex flex-col justify-center h-screen">
      <div class="flex flex-col justify-center items-center align">
        <button onClick={() => carControl('maju')} class="box rounded-2xl bg-blue-300 p-3 m-1 w-32 hover:bg-blue-400">Maju</button>
        <div>
          <button onClick={() => carControl('kiri')} class="box rounded-2xl bg-blue-300 p-3 m-1 w-32 hover:bg-blue-400">Kiri</button>
          <button onClick={() => carControl('kanan')} class="box rounded-2xl bg-blue-300 p-3 m-1 w-32 hover:bg-blue-400">Kanan</button>
        </div>
        <button onClick={() => carControl('mundur')} class="box rounded-2xl bg-blue-300 p-3 m-1 w-32 hover:bg-blue-400">Mundur</button>
      </div>

      <div class="flex justify-center">
        <h3>Current Speed : {speed ? `${speed}` : "Loading..."}</h3>
      </div>
    </div>
  )
}

export default App;