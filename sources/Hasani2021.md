[Liquid Time-Constant Networks](https://arxiv.org/pdf/2006.04439)
We introduce a new class of time-continuous recurrent neural network models. Instead of declaring a learning system’s dynamics by implicit nonlinearities, we construct networks of linear first-order dynamical systems modulated via nonlinear interlinked gates. The resulting models represent dynamical systems with varying (i.e., liquid) time-constants coupled to their hidden state, with outputs being computed by numerical differential equation solvers. These neural networks exhibit stable and bounded behavior, yield superior expressivity within the family of neural ordinary differential equations, and give rise to improved performance on time-series prediction tasks. To demonstrate these properties, we first take a theoretical approach to find bounds over their dynamics, and compute their expressive power by the trajectory length measure in a latent trajectory space. We then conduct a series of time-series prediction experiments to manifest the approximation capability of Liquid Time-Constant Networks (LTCs) compared to classical and modern RNNs
___
repeated proofs of generalization for LTCs as universal DS approximators
* reports on better long-term trajectories using PCA as comparison
* uses BPTT with different computational methods
* does not use evolution in any way, but instead relies on supervised learning via BPTT
* links different neural units together inspired from neurons and synaptic connections being the "trained liquid connections"
* Questions/Directions:
	* How much has Vivum used these vs. CTRNNs?
	* When evolving, are the mutation rates for the the different components and connections between reservoirs?
	* What is the scale of the ANNs (CTRNN and LTC) that Vivum has been working with?
	* What is the minimum scale to get some kind of working demo?
	* What 
___
creation date: 2025-06-13 08:11
___
