import { useEffect, useState } from 'react'
import { BinApi } from './api';
import { BinRecord } from '../../src/shared';

function App() {
	const [binData, setBinData] = useState<BinRecord[]>([]);

    const binFormatted = <ul>
        { binData.map(d => (<li key={d.id}>{d.id}: {d.fillLevel}</li>)) }
    </ul>;

	useEffect(() => {
		const timer = setInterval(() => {
			BinApi.getAll().then(setBinData);
		}, 1000);
	
		return () => clearTimeout(timer);
	}, []);
	

	return (
		<>
			{ binFormatted }
		</>
	)
}

export default App
