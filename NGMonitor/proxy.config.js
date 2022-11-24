const proxy = [
  {
    context: '/api/*',
    target: 'http://localhost:10102',
    changeOrigin: true,
    pathRewrite: {
      '^/api': ''
    }
  }
];

module.exports = proxy;