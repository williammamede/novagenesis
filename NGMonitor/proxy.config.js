const proxy = [
  {
    context: '/api',
    target: 'http://localhost:10102',
    changeOrigin: true,
  }
];

module.exports = proxy;